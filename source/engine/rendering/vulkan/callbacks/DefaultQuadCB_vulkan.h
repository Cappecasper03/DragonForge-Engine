#pragma once

#include <tracy/Tracy.hpp>

#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/vulkan/assets/cQuad_vulkan.h"
#include "engine/rendering/vulkan/assets/cTexture_vulkan.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/rendering/vulkan/pipeline/cPipeline_vulkan.h"

namespace df::vulkan::render_callback
{
	inline void defaultQuad( const cPipeline_vulkan* _pipeline, const cQuad_vulkan* _quad )
	{
		ZoneScoped;

		cRenderer_vulkan*     renderer       = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData&           frame_data     = renderer->getCurrentFrame();
		const VkCommandBuffer command_buffer = frame_data.command_buffer;
		const cCamera*        camera         = cCameraManager::getInstance()->current;

		const sVertexSceneUniforms vertex_scene_uniforms = {
			.view_projection = camera->view_projection,
		};

		void* data;
		vmaMapMemory( renderer->memory_allocator, frame_data.vertex_scene_uniform_buffer.allocation, &data );
		std::memcpy( data, &vertex_scene_uniforms, sizeof( vertex_scene_uniforms ) );
		vmaUnmapMemory( renderer->memory_allocator, frame_data.vertex_scene_uniform_buffer.allocation );

		std::vector< VkDescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.descriptors.allocate( renderer->vertex_scene_uniform_layout ) );

		sDescriptorWriter_vulkan writer_scene;
		writer_scene.writeBuffer( 0, frame_data.vertex_scene_uniform_buffer.buffer, sizeof( vertex_scene_uniforms ), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );
		writer_scene.updateSet( descriptor_sets.back() );

		writer_scene.clear();
		descriptor_sets.push_back( frame_data.descriptors.allocate( cQuad_vulkan::texture_layout ) );
		writer_scene.writeImage( 0,
		                         reinterpret_cast< cTexture_vulkan* >( _quad->texture )->getImage().image_view,
		                         renderer->sampler_nearest,
		                         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		                         VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );
		writer_scene.updateSet( descriptor_sets.back() );

		vkCmdBindPipeline( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->pipeline );

		vkCmdBindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->layout, 0, static_cast< uint32_t >( descriptor_sets.size() ), descriptor_sets.data(), 0, nullptr );

		const cQuad_vulkan::sPushConstants push_constants{
			.world_matrix = _quad->transform->world,
			.color        = _quad->color,
		};

		vkCmdPushConstants( command_buffer, _pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		const VkBuffer         vertex_buffers[] = { _quad->vertex_buffer.buffer };
		constexpr VkDeviceSize offsets[]        = { 0 };
		vkCmdBindVertexBuffers( command_buffer, 0, 1, vertex_buffers, offsets );

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
