#include "sFrameData_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/graphics/vulkan/descriptor/cDescriptorLayoutBuilder_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cLightManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "Helper_vulkan.h"

namespace df::vulkan
{
	vk::UniqueDescriptorSetLayout sFrameData_vulkan::s_vertex_scene_descriptor_set_layout   = {};
	vk::UniqueDescriptorSetLayout sFrameData_vulkan::s_fragment_scene_descriptor_set_layout = {};

	void sFrameData_vulkan::create()
	{
		DF_ProfilingScopeCpu;

		create( reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() ) );
	}

	void sFrameData_vulkan::create( const cGraphicsApi_vulkan* _graphics_api )
	{
		DF_ProfilingScopeCpu;

		const vk::Device& logical_device = _graphics_api->getLogicalDevice();

		const vk::CommandPoolCreateInfo command_pool_create_info = helper::init::commandPoolCreateInfo( _graphics_api->getGraphicsQueueFamily() );
		const vk::SemaphoreCreateInfo   semaphore_create_info    = helper::init::semaphoreCreateInfo();
		const vk::FenceCreateInfo       fence_create_info        = helper::init::fenceCreateInfo();

		command_pool = logical_device.createCommandPoolUnique( command_pool_create_info ).value;

		command_buffer.create( command_pool.get(), _graphics_api );

		swapchain_semaphore = logical_device.createSemaphoreUnique( semaphore_create_info ).value;
		render_semaphore    = logical_device.createSemaphoreUnique( semaphore_create_info ).value;
		render_fence        = logical_device.createFenceUnique( fence_create_info ).value;

		vertex_scene_uniform_buffer_3d.create( sizeof( sVertexSceneUniforms ), vk::BufferUsageFlagBits::eUniformBuffer, vma::MemoryUsage::eCpuToGpu );

		vertex_scene_uniform_buffer_2d.create( sizeof( sVertexSceneUniforms ), vk::BufferUsageFlagBits::eUniformBuffer, vma::MemoryUsage::eCpuToGpu );

		fragment_scene_uniform_buffer.create( sizeof( sLight ) * cLightManager::m_max_lights + sizeof( unsigned ),
		                                      vk::BufferUsageFlagBits::eUniformBuffer,
		                                      vma::MemoryUsage::eCpuToGpu );

		std::vector< cDescriptorAllocator_vulkan::sPoolSizeRatio > frame_sizes{
			{ vk::DescriptorType::eStorageImage,         3 },
			{ vk::DescriptorType::eStorageBuffer,        3 },
			{ vk::DescriptorType::eUniformBuffer,        3 },
			{ vk::DescriptorType::eCombinedImageSampler, 3 },
		};

		static_descriptors.create( logical_device, 1000, frame_sizes );
		dynamic_descriptors.create( logical_device, 1000, frame_sizes );

		if( !s_vertex_scene_descriptor_set_layout && !s_fragment_scene_descriptor_set_layout )
		{
			cDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
			descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eUniformBuffer );
			s_vertex_scene_descriptor_set_layout = descriptor_layout_builder.build( logical_device, vk::ShaderStageFlagBits::eVertex );

			descriptor_layout_builder.clear();
			descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eUniformBuffer );
			s_fragment_scene_descriptor_set_layout = descriptor_layout_builder.build( logical_device, vk::ShaderStageFlagBits::eFragment );
		}

		vertex_scene_descriptor_set_3d = static_descriptors.allocate( s_vertex_scene_descriptor_set_layout.get() );
		vertex_scene_descriptor_set_2d = static_descriptors.allocate( s_vertex_scene_descriptor_set_layout.get() );
		fragment_scene_descriptor_set  = static_descriptors.allocate( s_fragment_scene_descriptor_set_layout.get() );

#ifdef DF_Profiling
		profiling_context = TracyVkContextCalibrated( _graphics_api->getInstance(),
		                                              _graphics_api->getPhysicalDevice(),
		                                              logical_device,
		                                              _graphics_api->getGraphicsQueue(),
		                                              command_buffer.get(),
		                                              _graphics_api->getInstanceProcAddr(),
		                                              _graphics_api->getDeviceProcAddr() );
#endif
	}

	void sFrameData_vulkan::destroy()
	{
		DF_ProfilingScopeCpu;

		DF_DestroyProfilingContext( profiling_context );

		if( s_fragment_scene_descriptor_set_layout )
			s_fragment_scene_descriptor_set_layout.reset();

		if( s_vertex_scene_descriptor_set_layout )
			s_vertex_scene_descriptor_set_layout.reset();

		dynamic_descriptors.destroy();
		static_descriptors.destroy();

		fragment_scene_uniform_buffer.destroy();
		vertex_scene_uniform_buffer_2d.destroy();
		vertex_scene_uniform_buffer_3d.destroy();

		render_fence.reset();
		render_semaphore.reset();
		swapchain_semaphore.reset();

		command_buffer.destroy();
		command_pool.reset();
	}

	sAllocatedBuffer_vulkan& sFrameData_vulkan::getVertexSceneBuffer()
	{
		DF_ProfilingScopeCpu;

		return cCameraManager::getInstance()->m_current->getType() == cCamera::kPerspective ? vertex_scene_uniform_buffer_3d : vertex_scene_uniform_buffer_2d;
	}

	const sAllocatedBuffer_vulkan& sFrameData_vulkan::getVertexSceneBuffer() const
	{
		DF_ProfilingScopeCpu;

		return cCameraManager::getInstance()->m_current->getType() == cCamera::kPerspective ? vertex_scene_uniform_buffer_3d : vertex_scene_uniform_buffer_2d;
	}

	const vk::DescriptorSet& sFrameData_vulkan::getVertexDescriptorSet() const
	{
		DF_ProfilingScopeCpu;

		return cCameraManager::getInstance()->m_current->getType() == cCamera::kPerspective ? vertex_scene_descriptor_set_3d : vertex_scene_descriptor_set_2d;
	}
}
