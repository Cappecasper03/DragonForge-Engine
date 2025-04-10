﻿#pragma once

#include "engine/misc/cTransform.h"
// #include "engine/profiling/ProfilingMacros_vulkan.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/vulkan/assets/cMesh_vulkan.h"
#include "engine/rendering/vulkan/assets/cTexture_vulkan.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/rendering/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/rendering/vulkan/types/Helper_vulkan.h"
#include "engine/rendering/vulkan/types/sVertexSceneUniforms_vulkan.h"

namespace df::vulkan::render_callback
{
	inline void forwardMeshAmbient( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		// DF_ProfilingScopeCpu;
		cRenderer_vulkan*  renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		// DF_ProfilingScopeGpu( frame_data.tracy_context, frame_data.command_buffer.get() );

		const vk::UniqueCommandBuffer& command_buffer = frame_data.command_buffer;

		sAllocatedBuffer_vulkan& scene_uniform_buffer = cCameraManager::getInstance()->current->type == cCamera::ePerspective ? frame_data.vertex_scene_uniform_buffer_3d
		                                                                                                                      : frame_data.vertex_scene_uniform_buffer_2d;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.descriptors.allocate( cMesh_vulkan::getLayout() ) );

		sDescriptorWriter_vulkan writer_scene;
		writer_scene.writeBuffer( 0, scene_uniform_buffer.buffer.get(), sizeof( sVertexSceneUniforms_vulkan ), 0, vk::DescriptorType::eUniformBuffer );
		writer_scene.writeImage( 1,
		                         reinterpret_cast< cTexture_vulkan* >( _mesh->getTextures().at( aiTextureType_DIFFUSE ) )->getImage().image_view.get(),
		                         vk::ImageLayout::eShaderReadOnlyOptimal,
		                         vk::DescriptorType::eSampledImage );
		writer_scene.writeSampler( 2, renderer->getNearestSampler(), vk::DescriptorType::eSampler );
		writer_scene.updateSet( descriptor_sets.back() );

		command_buffer->bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline->pipeline.get() );
		command_buffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics,
		                                    _pipeline->layout.get(),
		                                    0,
		                                    static_cast< uint32_t >( descriptor_sets.size() ),
		                                    descriptor_sets.data(),
		                                    0,
		                                    nullptr );

		const cMesh_vulkan::sPushConstants push_constants{
			.world_matrix = _mesh->transform->world,
		};

		command_buffer->pushConstants( _pipeline->layout.get(), vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		const vk::Buffer         vertex_buffers[] = { _mesh->vertex_buffer.buffer.get() };
		constexpr vk::DeviceSize offsets[]        = { 0 };
		command_buffer->bindVertexBuffers( 0, 1, vertex_buffers, offsets );

		command_buffer->bindIndexBuffer( _mesh->index_buffer.buffer.get(), 0, vk::IndexType::eUint32 );

		command_buffer->drawIndexed( static_cast< uint32_t >( _mesh->getIndices().size() ), 1, 0, 0, 0 );
	}

	inline void forwardMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		// DF_ProfilingScopeCpu;

		const std::string_view name( _pipeline->getName() );

		if( name.find( "ambient" ) != std::string::npos )
			forwardMeshAmbient( _pipeline, _mesh );
	}

	inline void deferredMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		// DF_ProfilingScopeCpu;
		cRenderer_vulkan*  renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		// DF_ProfilingScopeGpu( frame_data.tracy_context, frame_data.command_buffer.get() );

		const vk::UniqueCommandBuffer& command_buffer = frame_data.command_buffer;
		const cCamera*                 camera         = cCameraManager::getInstance()->current;

		const sAllocatedBuffer_vulkan& vertex_scene_buffer = camera->type == cCamera::ePerspective ? frame_data.vertex_scene_uniform_buffer_3d
		                                                                                           : frame_data.vertex_scene_uniform_buffer_2d;

		const sVertexSceneUniforms_vulkan vertex_scene_uniforms{
			.view_projection = camera->view_projection,
		};

		void* data_dst = renderer->getMemoryAllocator().mapMemory( vertex_scene_buffer.allocation.get() ).value;
		std::memcpy( data_dst, &vertex_scene_uniforms, sizeof( vertex_scene_uniforms ) );
		renderer->getMemoryAllocator().unmapMemory( vertex_scene_buffer.allocation.get() );

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.descriptors.allocate( cMesh_vulkan::getLayout() ) );

		sDescriptorWriter_vulkan writer_scene;
		writer_scene.writeBuffer( 0, vertex_scene_buffer.buffer.get(), sizeof( vertex_scene_uniforms ), 0, vk::DescriptorType::eUniformBuffer );
		writer_scene.writeImage( 1,
		                         reinterpret_cast< cTexture_vulkan* >( _mesh->getTextures().at( aiTextureType_DIFFUSE ) )->getImage().image_view.get(),
		                         vk::ImageLayout::eShaderReadOnlyOptimal,
		                         vk::DescriptorType::eSampledImage );
		writer_scene.writeImage( 2,
		                         reinterpret_cast< cTexture_vulkan* >( _mesh->getTextures().at( aiTextureType_NORMALS ) )->getImage().image_view.get(),
		                         vk::ImageLayout::eShaderReadOnlyOptimal,
		                         vk::DescriptorType::eSampledImage );
		writer_scene.writeImage( 3,
		                         reinterpret_cast< cTexture_vulkan* >( _mesh->getTextures().at( aiTextureType_SPECULAR ) )->getImage().image_view.get(),
		                         vk::ImageLayout::eShaderReadOnlyOptimal,
		                         vk::DescriptorType::eSampledImage );
		writer_scene.writeSampler( 4, renderer->getNearestSampler(), vk::DescriptorType::eSampler );
		writer_scene.updateSet( descriptor_sets.back() );

		command_buffer->bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline->pipeline.get() );
		command_buffer->bindDescriptorSets( vk::PipelineBindPoint::eGraphics,
		                                    _pipeline->layout.get(),
		                                    0,
		                                    static_cast< uint32_t >( descriptor_sets.size() ),
		                                    descriptor_sets.data(),
		                                    0,
		                                    nullptr );

		const cMesh_vulkan::sPushConstants push_constants{
			.world_matrix = _mesh->transform->world,
		};

		command_buffer->pushConstants( _pipeline->layout.get(), vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		const vk::Buffer         vertex_buffers[] = { _mesh->vertex_buffer.buffer.get() };
		constexpr vk::DeviceSize offsets[]        = { 0 };
		command_buffer->bindVertexBuffers( 0, 1, vertex_buffers, offsets );

		command_buffer->bindIndexBuffer( _mesh->index_buffer.buffer.get(), 0, vk::IndexType::eUint32 );

		command_buffer->drawIndexed( static_cast< uint32_t >( _mesh->getIndices().size() ), 1, 0, 0, 0 );
	}
}
