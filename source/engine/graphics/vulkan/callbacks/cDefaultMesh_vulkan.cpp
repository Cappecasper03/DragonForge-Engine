#include "cDefaultMesh_vulkan.h"

#include "graphics/cRenderer.h"
#include "graphics/vulkan/assets/cTexture_vulkan.h"
#include "graphics/vulkan/cRenderer_vulkan.h"
#include "graphics/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "graphics/vulkan/types/sVertexSceneUniforms_vulkan.h"
#include "managers/assets/cCameraManager.h"
#include "misc/cTransform.h"
#include "profiling/ProfilingMacros.h"

namespace df::vulkan::render_callbacks
{
	void cDefaultMesh_vulkan::forwardMeshAmbient( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		DF_ProfilingScopeCpu;
		cRenderer_vulkan*        renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getDescriptorSet() );
		descriptor_sets.push_back( _mesh->getDescriptors()[ renderer->getCurrentFrameIndex() ] );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cMesh_vulkan::sPushConstants push_constants{
			.world_matrix = _mesh->transform->world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _mesh->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _mesh->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _mesh->getIndices().size() ), 1, 0, 0, 0 );
	}

	void cDefaultMesh_vulkan::forwardMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		DF_ProfilingScopeCpu;

		const std::string_view name( _pipeline->getName() );

		if( name.find( "ambient" ) != std::string::npos )
			forwardMeshAmbient( _pipeline, _mesh );
	}

	void cDefaultMesh_vulkan::deferredMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		DF_ProfilingScopeCpu;
		cRenderer_vulkan*  renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;
		const cCamera*        camera         = cCameraManager::getInstance()->current;

		const sAllocatedBuffer_vulkan& vertex_scene_buffer = camera->type == cCamera::kPerspective ? frame_data.vertex_scene_uniform_buffer_3d
		                                                                                           : frame_data.vertex_scene_uniform_buffer_2d;

		const sVertexSceneUniforms_vulkan vertex_scene_uniforms{
			.view_projection = camera->view_projection,
		};

		void* data_dst = renderer->getMemoryAllocator().mapMemory( vertex_scene_buffer.allocation.get() ).value;
		std::memcpy( data_dst, &vertex_scene_uniforms, sizeof( vertex_scene_uniforms ) );
		renderer->getMemoryAllocator().unmapMemory( vertex_scene_buffer.allocation.get() );

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.dynamic_descriptors.allocate( cMesh_vulkan::getLayout() ) );

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

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cMesh_vulkan::sPushConstants push_constants{
			.world_matrix = _mesh->transform->world,
		};

		command_buffer.pushConstants( _pipeline, vk::ShaderStageFlagBits::eVertex, 0, sizeof( push_constants ), &push_constants );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _mesh->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _mesh->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _mesh->getIndices().size() ), 1, 0, 0, 0 );
	}
}