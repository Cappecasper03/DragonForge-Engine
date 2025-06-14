#include "cMesh_vulkan.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <filesystem>

#include "cModel_vulkan.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorWriter_vulkan.h"
#include "engine/graphics/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "textures/cTexture2D_vulkan.h"

namespace df::vulkan
{
	vk::UniqueDescriptorSetLayout cMesh_vulkan::s_descriptor_layout = {};

	cMesh_vulkan::cMesh_vulkan( const aiMesh* _mesh, const aiScene* _scene, cModel_vulkan* _parent )
		: iMesh( _mesh, _scene, _parent )
	{
		DF_ProfilingScopeCpu;

		cMesh_vulkan::createTextures( _mesh, _scene );

		cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		const size_t vertex_buffer_size = sizeof( *m_vertices.data() ) * m_vertices.size();
		const size_t index_buffer_size  = sizeof( *m_indices.data() ) * m_indices.size();

		m_vertex_buffer.create( vertex_buffer_size, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vma::MemoryUsage::eGpuOnly );
		m_index_buffer.create( index_buffer_size, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, vma::MemoryUsage::eGpuOnly );

		sAllocatedBuffer_vulkan staging_buffer{};
		staging_buffer.create( vertex_buffer_size + index_buffer_size, vk::BufferUsageFlagBits::eTransferSrc, vma::MemoryUsage::eCpuOnly );

		void* data_dst = graphics_api->getMemoryAllocator().mapMemory( staging_buffer.allocation.get() ).value;
		std::memcpy( data_dst, m_vertices.data(), vertex_buffer_size );
		std::memcpy( static_cast< char* >( data_dst ) + vertex_buffer_size, m_indices.data(), index_buffer_size );
		graphics_api->getMemoryAllocator().unmapMemory( staging_buffer.allocation.get() );

		graphics_api->immediateSubmit(
			[ & ]( const vk::CommandBuffer _command_buffer )
			{
				const vk::BufferCopy vertex_copy( 0, 0, vertex_buffer_size );
				_command_buffer.copyBuffer( staging_buffer.buffer.get(), m_vertex_buffer.buffer.get(), 1, &vertex_copy );

				const vk::BufferCopy index_copy( vertex_buffer_size, 0, index_buffer_size );
				_command_buffer.copyBuffer( staging_buffer.buffer.get(), m_index_buffer.buffer.get(), 1, &index_copy );
			} );

		cDescriptorWriter_vulkan writer_scene;
		for( sFrameData_vulkan& frame_data: graphics_api->getFrameData() )
		{
			m_descriptors.push_back( frame_data.static_descriptors.allocate( s_descriptor_layout.get() ) );

			writer_scene.writeSampler( 0, graphics_api->getLinearSampler(), vk::DescriptorType::eSampler );
			writer_scene.writeImage( 1,
			                         reinterpret_cast< cTexture2D_vulkan* >( m_textures.at( aiTextureType_DIFFUSE ).get() )->getImage().image_view.get(),
			                         vk::ImageLayout::eShaderReadOnlyOptimal,
			                         vk::DescriptorType::eSampledImage );
			writer_scene.writeImage( 2,
			                         reinterpret_cast< cTexture2D_vulkan* >( m_textures.at( aiTextureType_NORMALS ).get() )->getImage().image_view.get(),
			                         vk::ImageLayout::eShaderReadOnlyOptimal,
			                         vk::DescriptorType::eSampledImage );
			writer_scene.updateSet( m_descriptors.back() );
		}
	}

	void cMesh_vulkan::render()
	{
		DF_ProfilingScopeCpu;

		if( cModelManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cPipeline_vulkan >( cModelManager::getForcedRenderCallback(), this );
		else if( m_render_callback )
			cRenderCallbackManager::render< cPipeline_vulkan >( m_render_callback.get(), this );
		else
			cRenderCallbackManager::render< cPipeline_vulkan >( cModelManager::getDefaultRenderCallback(), this );
	}

	void cMesh_vulkan::createTextures( const aiMesh* _mesh, const aiScene* _scene )
	{
		DF_ProfilingScopeCpu;

		const aiMaterial* material = _scene->mMaterials[ _mesh->mMaterialIndex ];

		for( const aiTextureType& texture_type: { aiTextureType_DIFFUSE, aiTextureType_NORMALS, aiTextureType_SPECULAR } )
		{
			for( unsigned i = 0; i < material->GetTextureCount( texture_type ); ++i )
			{
				aiString path;
				material->GetTexture( texture_type, i, &path );

				std::filesystem::path file_path    = m_parent->m_path;
				std::filesystem::path filename     = std::filesystem::path( path.data ).filename();
				const std::string     full_path    = fmt::format( "{}/{}", file_path.parent_path().string(), filename.string() );
				const std::string     texture_name = filename.replace_extension().string();

				if( auto it = m_parent->m_textures.find( full_path ); it != m_parent->m_textures.end() && it->second )
				{
					m_textures[ texture_type ] = it->second;
					continue;
				}

				const cTexture2D::sImageInfo   image_info = cTexture2D::getInfoFromFile( full_path );
				const cTexture2D::sDescription description{
					.name       = texture_name,
					.size       = image_info.size,
					.mip_levels = 1,
					.format     = image_info.format == sTextureFormat::kRGB ? sTextureFormat::kRGBA : image_info.format,
					.usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination,
				};
				cShared texture = cTexture2D::create( description );
				if( !texture->uploadDataFromFile( full_path, texture->getFormat() ) )
					continue;

				m_textures[ texture_type ]        = texture;
				m_parent->m_textures[ full_path ] = texture;
			}

			if( m_textures.contains( texture_type ) )
				continue;

			if( auto it = m_parent->m_textures.find( "df_white" ); it != m_parent->m_textures.end() && it->second )
			{
				m_textures[ texture_type ] = it->second;
				continue;
			}

			const cTexture2D::sDescription description{
				.name       = "df_white",
				.size       = cVector2u( 1 ),
				.mip_levels = 1,
				.format     = sTextureFormat::kRed,
				.usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination,
			};
			cShared texture                    = cTexture2D::create( description );
			m_textures[ texture_type ]         = texture;
			m_parent->m_textures[ "df_white" ] = texture;
		}
	}
}
