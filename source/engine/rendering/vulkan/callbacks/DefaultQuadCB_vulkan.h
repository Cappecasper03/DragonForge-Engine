#pragma once

#include <tracy/Tracy.hpp>

#include "engine/rendering/vulkan/assets/cQuad_vulkan.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/rendering/vulkan/pipeline/cPipeline_vulkan.h"

namespace df::vulkan::render_callback
{
	inline void defaultQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		ZoneScoped;

		cRenderer_vulkan*     render_instance = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData&           frame_data      = render_instance->getCurrentFrame();
		const VkCommandBuffer command_buffer  = render_instance->current_render_command_buffer;
		const VkExtent2D      render_extent   = render_instance->getRenderExtent();
		const cCamera*        camera          = cCameraManager::getInstance()->current;

		render_instance->vertex_scene_constants = {
			.view            = camera->view,
			.projection      = camera->projection,
			.view_projection = camera->view_projection,
		};

		sVertexSceneConstants* vertex_scene_constants = static_cast< sVertexSceneConstants* >( frame_data.vertex_scene_buffer.allocation_info.pMappedData );
		*vertex_scene_constants                       = render_instance->vertex_scene_constants;

		const VkDescriptorSet vertex_scene_descriptor = frame_data.descriptors.allocate( render_instance->vertex_scene_constants_layout );

		sDescriptorWriter_vulkan writer;
		writer.writeBuffer( 0, frame_data.vertex_scene_buffer.buffer, sizeof( sVertexSceneConstants ), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );
		writer.updateSet( render_instance->logical_device, vertex_scene_descriptor );

		vkCmdBindPipeline( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->pipeline );

		const VkViewport viewport{
			.x        = 0,
			.y        = 0,
			.width    = static_cast< float >( render_extent.width ),
			.height   = static_cast< float >( render_extent.height ),
			.minDepth = 0,
			.maxDepth = 1,
		};

		vkCmdSetViewport( command_buffer, 0, 1, &viewport );

		const VkRect2D scissor{
			.offset = {
				.x = 0,
				.y = 0,
			},
			.extent = {
				.width  = render_extent.width,
				.height = render_extent.height,
			},
		};

		vkCmdSetScissor( command_buffer, 0, 1, &scissor );

		const cQuad_vulkan::sVertexConstants vertex_constants{
			.u_world_matrix = _quad->transform->world,
			.vertex_buffer  = _quad->vertex_buffer_address,
		};

		vkCmdBindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->layout, 0, 1, &vertex_scene_descriptor, 0, nullptr );

		vkCmdPushConstants( command_buffer, _pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( vertex_constants ), &vertex_constants );
		vkCmdBindIndexBuffer( command_buffer, _quad->index_buffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

		vkCmdDrawIndexed( command_buffer, static_cast< uint32_t >( _quad->getIndices().size() ), 1, 0, 0, 0 );
	}

	inline void defaultQuadDeferred( const cPipeline_vulkan* /*_pipeline*/, const cQuad_vulkan* /*_quad*/ )
	{
		ZoneScoped;

		// const cFramebuffer* render_framebuffer = reinterpret_cast< const cFramebuffer* >( cRenderer::getRenderInstance()->getFramebuffer() );
		// const cCamera*      camera             = cCameraManager::getInstance()->current;
	}
}
