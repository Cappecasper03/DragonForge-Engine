#include "cDefaultMesh_vulkan.h"

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/lights/sLight.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/vulkan/assets/cTexture_vulkan.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"
#include "engine/graphics/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan::render_callbacks
{
	void cDefaultMesh_vulkan::forwardMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		DF_ProfilingScopeCpu;
		cRenderer_vulkan*        renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getVertexDescriptorSet() );
		descriptor_sets.push_back( _mesh->getDescriptors()[ renderer->getCurrentFrameIndex() ] );
		descriptor_sets.push_back( frame_data.fragment_scene_descriptor_set );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cMesh_vulkan::sPushConstants push_constants_fragment{
			.world_matrix    = _mesh->transform->world,
			.camera_position = cVector3f( cCameraManager::getInstance()->current->transform->world.position() ),
		};

		command_buffer.pushConstants( _pipeline,
		                              vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
		                              0,
		                              sizeof( push_constants_fragment ),
		                              &push_constants_fragment );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _mesh->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _mesh->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _mesh->getIndices().size() ), 1, 0, 0, 0 );
	}

	void cDefaultMesh_vulkan::deferredMesh( const cPipeline_vulkan* _pipeline, const cMesh_vulkan* _mesh )
	{
		DF_ProfilingScopeCpu;
		cRenderer_vulkan*        renderer   = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );
		const sFrameData_vulkan& frame_data = renderer->getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		std::vector< vk::DescriptorSet > descriptor_sets;
		descriptor_sets.push_back( frame_data.getVertexDescriptorSet() );
		descriptor_sets.push_back( _mesh->getDescriptors()[ renderer->getCurrentFrameIndex() ] );

		command_buffer.bindPipeline( vk::PipelineBindPoint::eGraphics, _pipeline );
		command_buffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, _pipeline, 0, descriptor_sets );

		const cMesh_vulkan::sPushConstants push_constants_fragment{
			.world_matrix    = _mesh->transform->world,
			.camera_position = cVector3f( cCameraManager::getInstance()->current->transform->world.position() ),
		};

		command_buffer.pushConstants( _pipeline,
		                              vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
		                              0,
		                              sizeof( push_constants_fragment ),
		                              &push_constants_fragment );

		renderer->setViewportScissor();

		command_buffer.bindVertexBuffers( 0, 1, _mesh->vertex_buffer, 0 );
		command_buffer.bindIndexBuffer( _mesh->index_buffer, 0, vk::IndexType::eUint32 );

		command_buffer.drawIndexed( static_cast< unsigned >( _mesh->getIndices().size() ), 1, 0, 0, 0 );
	}
}