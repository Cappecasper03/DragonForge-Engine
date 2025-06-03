#include "cMesh_vulkan.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <filesystem>

#include "cModel_vulkan.h"
#include "cTexture2D_vulkan.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorWriter_vulkan.h"
#include "engine/graphics/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	vk::UniqueDescriptorSetLayout cMesh_vulkan::s_descriptor_layout = {};

	cMesh_vulkan::cMesh_vulkan( const aiMesh* _mesh, const aiScene* _scene, cModel_vulkan* _parent )
		: iMesh( _mesh, _scene, _parent )
	{
		DF_ProfilingScopeCpu;

		cMesh_vulkan::createTextures( _mesh, _scene );

		cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		const size_t vertex_buffer_size = sizeof( *m_vertices.data() ) * m_vertices.size();
		const size_t index_buffer_size  = sizeof( *m_indices.data() ) * m_indices.size();

		m_vertex_buffer = helper::util::createBuffer( vertex_buffer_size,
		                                              vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		                                              vma::MemoryUsage::eGpuOnly );
		m_index_buffer = helper::util::createBuffer( index_buffer_size, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, vma::MemoryUsage::eGpuOnly );

		sAllocatedBuffer_vulkan staging_buffer = helper::util::createBuffer( vertex_buffer_size + index_buffer_size,
		                                                                     vk::BufferUsageFlagBits::eTransferSrc,
		                                                                     vma::MemoryUsage::eCpuOnly );

		void* data_dst = renderer->getMemoryAllocator().mapMemory( staging_buffer.allocation.get() ).value;
		std::memcpy( data_dst, m_vertices.data(), vertex_buffer_size );
		std::memcpy( static_cast< char* >( data_dst ) + vertex_buffer_size, m_indices.data(), index_buffer_size );
		renderer->getMemoryAllocator().unmapMemory( staging_buffer.allocation.get() );

		renderer->immediateSubmit(
			[ & ]( const vk::CommandBuffer _command_buffer )
			{
				const vk::BufferCopy vertex_copy( 0, 0, vertex_buffer_size );
				_command_buffer.copyBuffer( staging_buffer.buffer.get(), m_vertex_buffer.buffer.get(), 1, &vertex_copy );

				const vk::BufferCopy index_copy( vertex_buffer_size, 0, index_buffer_size );
				_command_buffer.copyBuffer( staging_buffer.buffer.get(), m_index_buffer.buffer.get(), 1, &index_copy );
			} );

		cDescriptorWriter_vulkan writer_scene;
		for( sFrameData_vulkan& frame_data: renderer->getFrameData() )
		{
			m_descriptors.push_back( frame_data.static_descriptors.allocate( s_descriptor_layout.get() ) );

			writer_scene.writeSampler( 0, renderer->getLinearSampler(), vk::DescriptorType::eSampler );
			writer_scene.writeImage( 1,
			                         reinterpret_cast< cTexture2D_vulkan* >( m_textures.at( aiTextureType_DIFFUSE ) )->getImage().image_view.get(),
			                         vk::ImageLayout::eShaderReadOnlyOptimal,
			                         vk::DescriptorType::eSampledImage );
			writer_scene.writeImage( 2,
			                         reinterpret_cast< cTexture2D_vulkan* >( m_textures.at( aiTextureType_NORMALS ) )->getImage().image_view.get(),
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
			cRenderCallbackManager::render< cPipeline_vulkan >( m_render_callback, this );
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

				cTexture2D_vulkan* texture = new cTexture2D_vulkan( texture_name );
				if( !texture->load( full_path ) )
				{
					delete texture;
					continue;
				}

				m_textures[ texture_type ]        = texture;
				m_parent->m_textures[ full_path ] = texture;
			}

			if( m_textures.contains( texture_type ) )
				continue;

			if( auto it = m_parent->m_textures.find( "white" ); it != m_parent->m_textures.end() && it->second )
			{
				m_textures[ texture_type ] = it->second;
				continue;
			}

			cTexture2D_vulkan* texture        = new cTexture2D_vulkan( "white" );
			m_textures[ texture_type ]      = texture;
			m_parent->m_textures[ "white" ] = texture;
		}
	}
}
