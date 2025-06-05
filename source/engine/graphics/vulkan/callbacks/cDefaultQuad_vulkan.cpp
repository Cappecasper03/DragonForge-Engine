#include "cDefaultQuad_vulkan.h"

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/assets/textures/cTexture2D_vulkan.h"
#include "engine/graphics/vulkan/cFramebuffer_vulkan.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorWriter_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan::render_callbacks
{
	void cDefaultQuad_vulkan::forwardQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		DF_ProfilingScopeCpu;
		cGraphicsDevice_vulkan*  renderer   = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getVertexDescriptorSet() );
		descriptor_sets.push_back( _quad->getDescriptors()[ renderer->getCurrentFrameIndex() ] );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cQuad_vulkan::sPushConstants push_constants{
			.world_matrix = _quad->m_transform.m_world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _quad->m_vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _quad->m_index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}

	void cDefaultQuad_vulkan::deferredQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		DF_ProfilingScopeCpu;
		cGraphicsDevice_vulkan*  renderer   = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getVertexDescriptorSet() );
		descriptor_sets.push_back( _quad->getDescriptors()[ renderer->getCurrentFrameIndex() ] );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cQuad_vulkan::sPushConstants push_constants{
			.world_matrix = _quad->m_transform.m_world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _quad->m_vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _quad->m_index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}

	void cDefaultQuad_vulkan::deferredQuadFinal( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		DF_ProfilingScopeCpu;
		cGraphicsDevice_vulkan*  renderer   = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cFramebuffer_vulkan*               framebuffer        = reinterpret_cast< const cFramebuffer_vulkan* >( renderer->getDeferredFramebuffer() );
		const std::vector< cTexture2D_vulkan* >& framebuffer_images = framebuffer->getCurrentFrameImages( renderer->getCurrentFrameIndex() );
		const cCommandBuffer&                    command_buffer     = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getVertexDescriptorSet() );
		descriptor_sets.push_back( renderer->getCurrentDescriptor() );

		cDescriptorWriter_vulkan writer_scene;
		writer_scene.writeSampler( 0, renderer->getLinearSampler(), vk::DescriptorType::eSampler );
		for( size_t i = 0; i < framebuffer_images.size(); ++i )
		{
			writer_scene.writeImage( static_cast< uint32_t >( i + 1 ),
			                         framebuffer_images[ i ]->getImage().image_view.get(),
			                         vk::ImageLayout::eShaderReadOnlyOptimal,
			                         vk::DescriptorType::eSampledImage );
		}
		writer_scene.updateSet( descriptor_sets.back() );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const iQuad::sPushConstants push_constants{
			.world_matrix = _quad->m_transform.m_world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _quad->m_vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _quad->m_index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}
}