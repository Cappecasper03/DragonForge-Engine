#include "sFrameData_vulkan.h"

#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "Helper_vulkan.h"
#include "profiling/ProfilingMacros.h"
#include "profiling/ProfilingMacros_vulkan.h"
#include "sVertexSceneUniforms_vulkan.h"

namespace df::vulkan
{
	void sFrameData_vulkan::create()
	{
		DF_ProfilingScopeCpu;

		create( reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() ) );
	}

	void sFrameData_vulkan::create( const cRenderer_vulkan* _renderer )
	{
		DF_ProfilingScopeCpu;

		const vk::Device& logical_device = _renderer->getLogicalDevice();

		const vk::CommandPoolCreateInfo command_pool_create_info = helper::init::commandPoolCreateInfo( _renderer->getGraphicsQueueFamily() );
		const vk::SemaphoreCreateInfo   semaphore_create_info    = helper::init::semaphoreCreateInfo();
		const vk::FenceCreateInfo       fence_create_info        = helper::init::fenceCreateInfo();

		command_pool = logical_device.createCommandPoolUnique( command_pool_create_info ).value;

		const vk::CommandBufferAllocateInfo command_buffer_allocate_info = helper::init::commandBufferAllocateInfo( command_pool.get() );
		command_buffer.swap( logical_device.allocateCommandBuffersUnique( command_buffer_allocate_info ).value.front() );

		swapchain_semaphore = logical_device.createSemaphoreUnique( semaphore_create_info ).value;
		render_semaphore    = logical_device.createSemaphoreUnique( semaphore_create_info ).value;
		render_fence        = logical_device.createFenceUnique( fence_create_info ).value;

		vertex_scene_uniform_buffer_3d = helper::util::createBuffer( sizeof( sVertexSceneUniforms_vulkan ),
		                                                             vk::BufferUsageFlagBits::eUniformBuffer,
		                                                             vma::MemoryUsage::eCpuToGpu,
		                                                             _renderer->getMemoryAllocator() );

		vertex_scene_uniform_buffer_2d = helper::util::createBuffer( sizeof( sVertexSceneUniforms_vulkan ),
		                                                             vk::BufferUsageFlagBits::eUniformBuffer,
		                                                             vma::MemoryUsage::eCpuToGpu,
		                                                             _renderer->getMemoryAllocator() );

		std::vector< sDescriptorAllocator_vulkan::sPoolSizeRatio > frame_sizes{
			{ vk::DescriptorType::eStorageImage,         3 },
			{ vk::DescriptorType::eStorageBuffer,        3 },
			{ vk::DescriptorType::eUniformBuffer,        3 },
			{ vk::DescriptorType::eCombinedImageSampler, 3 },
		};

		descriptors.create( logical_device, 1000, frame_sizes );

#ifdef DF_Profiling
		tracy_context = TracyVkContextCalibrated( _renderer->getInstance(),
		                                          _renderer->getPhysicalDevice(),
		                                          logical_device,
		                                          _renderer->getGraphicsQueue(),
		                                          command_buffer.get(),
		                                          _renderer->getInstanceProcAddr(),
		                                          _renderer->getDeviceProcAddr() );
#endif
	}

	void sFrameData_vulkan::destroy()
	{
		DF_ProfilingScopeCpu;

		DF_DestroyProfilingContext( tracy_context );

		descriptors.destroy();

		helper::util::destroyBuffer( vertex_scene_uniform_buffer_2d );
		helper::util::destroyBuffer( vertex_scene_uniform_buffer_3d );

		render_fence.reset();
		render_semaphore.reset();
		swapchain_semaphore.reset();

		command_buffer.reset();
		command_pool.reset();
	}
}
