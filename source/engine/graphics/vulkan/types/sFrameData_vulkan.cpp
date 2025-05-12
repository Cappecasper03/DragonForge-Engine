#include "sFrameData_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"
#include "engine/graphics/vulkan/descriptor/sDescriptorLayoutBuilder_vulkan.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cLightManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "Helper_vulkan.h"
#include "sSceneUniforms_vulkan.h"

namespace df::vulkan
{
	vk::UniqueDescriptorSetLayout sFrameData_vulkan::s_vertex_scene_descriptor_set_layout   = {};
	vk::UniqueDescriptorSetLayout sFrameData_vulkan::s_fragment_scene_descriptor_set_layout = {};

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

		command_buffer.create( command_pool.get(), _renderer );

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

		fragment_scene_uniform_buffer = helper::util::createBuffer( sizeof( sLight ) * cLightManager::m_max_lights + sizeof( unsigned ),
		                                                            vk::BufferUsageFlagBits::eUniformBuffer,
		                                                            vma::MemoryUsage::eCpuToGpu,
		                                                            _renderer->getMemoryAllocator() );

		std::vector< sDescriptorAllocator_vulkan::sPoolSizeRatio > frame_sizes{
			{ vk::DescriptorType::eStorageImage,         3 },
			{ vk::DescriptorType::eStorageBuffer,        3 },
			{ vk::DescriptorType::eUniformBuffer,        3 },
			{ vk::DescriptorType::eCombinedImageSampler, 3 },
		};

		static_descriptors.create( logical_device, 1000, frame_sizes );
		dynamic_descriptors.create( logical_device, 1000, frame_sizes );

		if( !s_vertex_scene_descriptor_set_layout && !s_fragment_scene_descriptor_set_layout )
		{
			sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
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
		profiling_context = TracyVkContextCalibrated( _renderer->getInstance(),
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

		DF_DestroyProfilingContext( profiling_context );

		if( s_fragment_scene_descriptor_set_layout )
			s_fragment_scene_descriptor_set_layout.reset();

		if( s_vertex_scene_descriptor_set_layout )
			s_vertex_scene_descriptor_set_layout.reset();

		dynamic_descriptors.destroy();
		static_descriptors.destroy();

		helper::util::destroyBuffer( fragment_scene_uniform_buffer );
		helper::util::destroyBuffer( vertex_scene_uniform_buffer_2d );
		helper::util::destroyBuffer( vertex_scene_uniform_buffer_3d );

		render_fence.reset();
		render_semaphore.reset();
		swapchain_semaphore.reset();

		command_buffer.destroy();
		command_pool.reset();
	}

	const sAllocatedBuffer_vulkan& sFrameData_vulkan::getVertexSceneBuffer() const
	{
		DF_ProfilingScopeCpu;

		return cCameraManager::getInstance()->current->type == cCamera::kPerspective ? vertex_scene_uniform_buffer_3d : vertex_scene_uniform_buffer_2d;
	}

	const vk::DescriptorSet& sFrameData_vulkan::getVertexDescriptorSet() const
	{
		DF_ProfilingScopeCpu;

		return cCameraManager::getInstance()->current->type == cCamera::kPerspective ? vertex_scene_descriptor_set_3d : vertex_scene_descriptor_set_2d;
	}
}
