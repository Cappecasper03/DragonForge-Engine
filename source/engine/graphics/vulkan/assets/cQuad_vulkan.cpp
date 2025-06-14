﻿#include "cQuad_vulkan.h"

#include "engine/graphics/callback/iRenderCallback.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/callbacks/cDefaultQuad_vulkan.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorLayoutBuilder_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorWriter_vulkan.h"
#include "engine/graphics/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "textures/cTexture2D_vulkan.h"

namespace df::vulkan
{
	vk::UniqueDescriptorSetLayout cQuad_vulkan::s_descriptor_layout = {};

	cQuad_vulkan::cQuad_vulkan( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color, const bool _update_descriptor_set )
		: iQuad( _name, _position, _size, _color )
	{
		DF_ProfilingScopeCpu;

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

		const cTexture2D::sDescription description{
			.name       = fmt::format( "{}_{}", m_name, "texture" ),
			.size       = cVector2u( 1 ),
			.mip_levels = 1,
			.format     = sTextureFormat::kRed,
			.usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination,
		};
		m_texture = cTexture2D::create( description );

		if( _update_descriptor_set )
		{
			cDescriptorWriter_vulkan writer_scene;
			for( sFrameData_vulkan& frame_data: renderer->getFrameData() )
			{
				m_descriptors.push_back( frame_data.static_descriptors.allocate( s_descriptor_layout.get() ) );

				writer_scene.clear();
				writer_scene.writeSampler( 0, renderer->getLinearSampler(), vk::DescriptorType::eSampler );
				writer_scene.writeImage( 1,
				                         reinterpret_cast< cTexture2D_vulkan* >( m_texture )->getImage().image_view.get(),
				                         vk::ImageLayout::eShaderReadOnlyOptimal,
				                         vk::DescriptorType::eSampledImage );
				writer_scene.updateSet( m_descriptors.back() );
			}
		}
	}

	bool cQuad_vulkan::loadTexture( const std::string& _file_path, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		const std::string full_path = cFileSystem::getPath( _file_path );

		const cTexture2D::sImageInfo   image_info = cTexture2D::getInfoFromFile( full_path );
		const cTexture2D::sDescription description{
			.name       = fmt::format( "{}_{}", m_name, "texture" ),
			.size       = image_info.size,
			.mip_levels = 1,
			.format     = image_info.format,
			.usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination,
		};

		delete m_texture;
		m_texture = cTexture2D::create( description );

		if( m_texture->uploadDataFromFile( full_path, m_texture->getFormat(), _mipmaps, _flip_vertically_on_load ) )
		{
			const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
			cDescriptorWriter_vulkan      writer_scene;
			for( const vk::DescriptorSet& descriptor: m_descriptors )
			{
				writer_scene.clear();
				writer_scene.writeSampler( 0, renderer->getLinearSampler(), vk::DescriptorType::eSampler );
				writer_scene.writeImage( 1,
				                         reinterpret_cast< cTexture2D_vulkan* >( m_texture )->getImage().image_view.get(),
				                         vk::ImageLayout::eShaderReadOnlyOptimal,
				                         vk::DescriptorType::eSampledImage );
				writer_scene.updateSet( descriptor );
			}

			return true;
		}

		return false;
	}

	void cQuad_vulkan::render()
	{
		DF_ProfilingScopeCpu;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cPipeline_vulkan >( cQuadManager::getForcedRenderCallback(), this );
		else if( m_render_callback )
			cRenderCallbackManager::render< cPipeline_vulkan >( m_render_callback, this );
		else
			cRenderCallbackManager::render< cPipeline_vulkan >( cQuadManager::getDefaultRenderCallback(), this );
	}

	iRenderCallback* cQuad_vulkan::createDefaults()
	{
		DF_ProfilingScopeCpu;

		if( cRenderer::isDeferred() )
			return createDefaultsDeferred();

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		cPipelineCreateInfo_vulkan pipeline_create_info{ .m_name = "forward_quad" };

		pipeline_create_info.m_vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.m_vertex_input_attribute.emplace_back( 0,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iQuad::sVertex, iQuad::sVertex::position ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 1,
		                                                            0,
		                                                            vk::Format::eR32G32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iQuad::sVertex, iQuad::sVertex::tex_coord ) ) );

		pipeline_create_info.m_push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( sPushConstants ) ) );

		cDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eSampler );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		s_descriptor_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.m_descriptor_layouts.push_back( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() );
		pipeline_create_info.m_descriptor_layouts.push_back( s_descriptor_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "forward_quad.vert" ), helper::util::createShaderModule( "forward_quad.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( renderer->getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		return cRenderCallbackManager::create( "forward_quad", pipeline_create_info, render_callbacks::cDefaultQuad_vulkan::forwardQuad );
	}

	void cQuad_vulkan::destroyDefaults()
	{
		DF_ProfilingScopeCpu;

		s_descriptor_layout.reset();
	}

	iRenderCallback* cQuad_vulkan::createDefaultsDeferred()
	{
		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		cPipelineCreateInfo_vulkan pipeline_create_info{ .m_name = "deferred_quad" };

		pipeline_create_info.m_vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.m_vertex_input_attribute.emplace_back( 0,
		                                                            0,
		                                                            vk::Format::eR32G32B32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iQuad::sVertex, iQuad::sVertex::position ) ) );
		pipeline_create_info.m_vertex_input_attribute.emplace_back( 1,
		                                                            0,
		                                                            vk::Format::eR32G32Sfloat,
		                                                            static_cast< uint32_t >( offsetof( iQuad::sVertex, iQuad::sVertex::tex_coord ) ) );

		pipeline_create_info.m_push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( sPushConstants ) ) );

		cDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eSampler );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		s_descriptor_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.m_descriptor_layouts.push_back( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() );
		pipeline_create_info.m_descriptor_layouts.push_back( s_descriptor_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "deferred_quad.vert" ), helper::util::createShaderModule( "deferred_quad.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormats( std::vector( 3, renderer->getRenderColorFormat() ) );
		pipeline_create_info.setDepthFormat( renderer->getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		return cRenderCallbackManager::create( "deferred_quad", pipeline_create_info, render_callbacks::cDefaultQuad_vulkan::deferredQuad );
	}
}
