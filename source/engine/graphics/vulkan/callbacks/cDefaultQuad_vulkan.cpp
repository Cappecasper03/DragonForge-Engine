#include "cDefaultQuad_vulkan.h"

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/vulkan/assets/cTexture_vulkan.h"
#include "engine/graphics/vulkan/cDeferredRenderer_vulkan.h"
#include "engine/graphics/vulkan/cFramebuffer_vulkan.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"
#include "engine/graphics/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan::render_callbacks
{
	void cDefaultQuad_vulkan::forwardQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		DF_ProfilingScopeCpu;
		cRenderer_vulkan*        renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getVertexDescriptorSet() );
		descriptor_sets.push_back( _quad->getDescriptors()[ renderer->getCurrentFrameIndex() ] );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cQuad_vulkan::sPushConstants push_constants{
			.world_matrix = _quad->transform->world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _quad->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _quad->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}

	void cDefaultQuad_vulkan::deferredQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		DF_ProfilingScopeCpu;
		cRenderer_vulkan*  renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		sAllocatedBuffer_vulkan& scene_uniform_buffer = cCameraManager::getInstance()->current->type == cCamera::kPerspective ? frame_data.vertex_scene_uniform_buffer_3d
		                                                                                                                      : frame_data.vertex_scene_uniform_buffer_2d;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.dynamic_descriptors.allocate( cQuad_vulkan::getLayout() ) );

		sDescriptorWriter_vulkan writer_scene;
		writer_scene.writeBuffer( 0, scene_uniform_buffer.buffer.get(), sizeof( sVertexSceneUniforms ), 0, vk::DescriptorType::eUniformBuffer );
		writer_scene.writeImage( 1,
		                         reinterpret_cast< cTexture_vulkan* >( _quad->texture )->getImage().image_view.get(),
		                         vk::ImageLayout::eShaderReadOnlyOptimal,
		                         vk::DescriptorType::eSampledImage );
		writer_scene.writeSampler( 2, renderer->getLinearSampler(), vk::DescriptorType::eSampler );
		writer_scene.updateSet( descriptor_sets.back() );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cQuad_vulkan::sPushConstants push_constants{
			.world_matrix = _quad->transform->world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _quad->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _quad->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}

	void cDefaultQuad_vulkan::deferredQuadFinal( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		DF_ProfilingScopeCpu;
		cDeferredRenderer_vulkan* renderer   = reinterpret_cast< cDeferredRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData_vulkan&        frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cFramebuffer_vulkan*                   framebuffer        = reinterpret_cast< const cFramebuffer_vulkan* >( renderer->getDeferredFramebuffer() );
		const std::vector< sAllocatedImage_vulkan >& framebuffer_images = framebuffer->getCurrentFrameImages( renderer->getCurrentFrameIndex() );
		const cCommandBuffer&                        command_buffer     = frame_data.command_buffer;
		const cCamera*                               camera             = cCameraManager::getInstance()->current;

		const sAllocatedBuffer_vulkan& vertex_scene_buffer = camera->type == cCamera::kPerspective ? frame_data.vertex_scene_uniform_buffer_3d
		                                                                                           : frame_data.vertex_scene_uniform_buffer_2d;

		const sVertexSceneUniforms vertex_scene_uniforms{
			.view_projection = camera->view_projection,
		};

		void* data_dst = renderer->getMemoryAllocator().mapMemory( vertex_scene_buffer.allocation.get() ).value;
		std::memcpy( data_dst, &vertex_scene_uniforms, sizeof( vertex_scene_uniforms ) );
		renderer->getMemoryAllocator().unmapMemory( vertex_scene_buffer.allocation.get() );

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.dynamic_descriptors.allocate( renderer->getDeferredLayout() ) );

		sDescriptorWriter_vulkan writer_scene;
		writer_scene.writeBuffer( 0, vertex_scene_buffer.buffer.get(), sizeof( vertex_scene_uniforms ), 0, vk::DescriptorType::eUniformBuffer );

		for( size_t i = 0; i < framebuffer_images.size(); ++i )
		{
			writer_scene.writeImage( static_cast< uint32_t >( i + 1 ),
			                         framebuffer_images[ i ].image_view.get(),
			                         vk::ImageLayout::eShaderReadOnlyOptimal,
			                         vk::DescriptorType::eSampledImage );
		}

		writer_scene.writeSampler( 4, renderer->getLinearSampler(), vk::DescriptorType::eSampler );
		writer_scene.updateSet( descriptor_sets.back() );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cDeferredRenderer_vulkan::sPushConstants push_constants{
			.world_matrix = _quad->transform->world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _quad->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _quad->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}
}