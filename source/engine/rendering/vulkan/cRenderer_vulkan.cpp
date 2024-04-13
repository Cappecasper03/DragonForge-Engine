#include "cRenderer_vulkan.h"

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <algorithm>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <set>
#include <VkBootstrap.h>

#include "descriptor/sDescriptorLayoutBuilder_vulkan.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "framework/application/cApplication.h"
#include "misc/Helper_vulkan.h"

namespace df::vulkan
{
	cRenderer_vulkan::cRenderer_vulkan()
		: m_frame_number( 0 )
		, m_frames( frame_overlap )
	{
		ZoneScoped;

		glfwInit();

		glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

		m_window = glfwCreateWindow( m_window_size.x, m_window_size.y, cApplication::getName().data(), nullptr, nullptr );
		if( !m_window )
			DF_LOG_ERROR( "Failed to create window" );
		else
			DF_LOG_MESSAGE( fmt::format( "Created window [{}, {}]", m_window_size.x, m_window_size.y ) );

		glfwSetWindowUserPointer( m_window, this );
		glfwSetFramebufferSizeCallback( m_window, framebufferSizeCallback );

		vkb::InstanceBuilder builder;
		builder.set_app_name( cApplication::getName().data() );
		builder.set_debug_callback( debugMessageCallback );
		builder.require_api_version( VK_API_VERSION_1_3 );

#ifdef DEBUG
		builder.request_validation_layers( true );
#else
		builder.request_validation_layers( false );
#endif

		vkb::Instance instance = builder.build().value();
		m_instance             = instance.instance;
		m_debug_messenger      = instance.debug_messenger;

		glfwCreateWindowSurface( m_instance, m_window, nullptr, &m_surface );

		VkPhysicalDeviceVulkan12Features features12{
			.descriptorIndexing  = true,
			.bufferDeviceAddress = true,
		};

		VkPhysicalDeviceVulkan13Features features13{
			.synchronization2 = true,
			.dynamicRendering = true,
		};

		vkb::PhysicalDeviceSelector selector( instance );
		selector.set_minimum_version( 1, 3 );
		selector.set_required_features_12( features12 );
		selector.set_required_features_13( features13 );
		selector.set_surface( m_surface );

		vkb::PhysicalDevice vkb_physical_device = selector.select().value();

		vkb::DeviceBuilder device_builder( vkb_physical_device );
		vkb::Device        vkb_logical_device = device_builder.build().value();

		physical_device = vkb_physical_device.physical_device;
		logical_device  = vkb_logical_device.device;

		m_graphics_queue        = vkb_logical_device.get_queue( vkb::QueueType::graphics ).value();
		m_graphics_queue_family = vkb_logical_device.get_queue_index( vkb::QueueType::graphics ).value();
		m_present_queue         = vkb_logical_device.get_queue( vkb::QueueType::present ).value();
		m_present_queue_family  = vkb_logical_device.get_queue_index( vkb::QueueType::present ).value();

		createMemoryAllocator();
		createSwapchain( m_window_size.x, m_window_size.y );
		createFrameDatas();
		createSubmitContext();

		sDescriptorLayoutBuilder_vulkan layout_builder;
		layout_builder.addBinding( 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );
		vertex_scene_constants_descriptor = layout_builder.build( logical_device, VK_SHADER_STAGE_VERTEX_BIT );

		DF_LOG_MESSAGE( "Initialized renderer" );
	}

	cRenderer_vulkan::~cRenderer_vulkan()
	{
		ZoneScoped;

		vkDeviceWaitIdle( logical_device );

		vkDestroyDescriptorSetLayout( logical_device, vertex_scene_constants_descriptor, nullptr );

		vkDestroyCommandPool( logical_device, m_submit_context.command_pool, nullptr );
		vkDestroyFence( logical_device, m_submit_context.fence, nullptr );

		vmaDestroyImage( memory_allocator, m_render_image.image, m_render_image.allocation );
		vkDestroyImageView( logical_device, m_render_image.image_view, nullptr );
		vmaDestroyImage( memory_allocator, m_depth_image.image, m_depth_image.allocation );
		vkDestroyImageView( logical_device, m_depth_image.image_view, nullptr );

		for( sFrameData& frame_data: m_frames )
		{
			frame_data.descriptors.destroy();

			helper::util::destroyBuffer( frame_data.vertex_scene_buffer );

			vkDestroyFence( logical_device, frame_data.render_fence, nullptr );
			vkDestroySemaphore( logical_device, frame_data.render_semaphore, nullptr );
			vkDestroySemaphore( logical_device, frame_data.swapchain_semaphore, nullptr );
			vkDestroyCommandPool( logical_device, frame_data.command_pool, nullptr );
		}

		for( const VkImageView& image_view: m_swapchain_image_views )
			vkDestroyImageView( logical_device, image_view, nullptr );

		vkDestroySwapchainKHR( logical_device, m_swapchain, nullptr );
		vmaDestroyAllocator( memory_allocator );
		vkDestroySurfaceKHR( m_instance, m_surface, nullptr );
		vkDestroyDevice( logical_device, nullptr );

		vkb::destroy_debug_utils_messenger( m_instance, m_debug_messenger );
		vkDestroyInstance( m_instance, nullptr );
		glfwDestroyWindow( m_window );

		glfwTerminate();

		DF_LOG_MESSAGE( "Deinitialized renderer" );
	}

	void cRenderer_vulkan::render()
	{
		ZoneScoped;

		sFrameData&           frame_data          = getCurrentFrame();
		const VkCommandBuffer command_buffer      = frame_data.command_buffer;
		const VkSemaphore     swapchain_semaphore = frame_data.swapchain_semaphore;
		const VkFence         render_fence        = frame_data.render_fence;

		vkWaitForFences( logical_device, 1, &render_fence, true, UINT64_MAX );
		frame_data.descriptors.clear();

		uint32_t swapchain_image_index;
		VkResult result = vkAcquireNextImageKHR( logical_device, m_swapchain, UINT64_MAX, swapchain_semaphore, nullptr, &swapchain_image_index );

		if( result == VK_ERROR_OUT_OF_DATE_KHR )
		{
			resize();
			return;
		}

		if( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
		{
			DF_LOG_ERROR( "Failed to acquire next image" );
			return;
		}

		vkResetFences( logical_device, 1, &render_fence );
		vkResetCommandBuffer( command_buffer, 0 );

		const VkCommandBufferBeginInfo command_buffer_begin_info = helper::init::commandBufferBeginInfo( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		if( vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to begin command buffer" );
			return;
		}

		helper::util::transitionImage( command_buffer, m_render_image.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL );

		current_render_command_buffer = command_buffer;
		cEventManager::invoke( event::render_3d );
		cEventManager::invoke( event::render_2d );
		current_render_command_buffer = nullptr;

		helper::util::transitionImage( command_buffer, m_render_image.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL );
		helper::util::transitionImage( command_buffer, m_swapchain_images[ swapchain_image_index ], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );

		helper::util::copyImageToImage( command_buffer, m_render_image.image, m_swapchain_images[ swapchain_image_index ], m_render_extent, m_swapchain_extent );

		helper::util::transitionImage( command_buffer, m_swapchain_images[ swapchain_image_index ], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR );

		if( vkEndCommandBuffer( command_buffer ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to end command buffer" );
			return;
		}

		const std::vector                         wait_semaphores   = { swapchain_semaphore };
		const std::vector                         signal_semaphores = { frame_data.render_semaphore };
		const std::vector< VkPipelineStageFlags > wait_stages       = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submit_info         = helper::init::submitInfo( command_buffer );
		submit_info.waitSemaphoreCount   = static_cast< uint32_t >( wait_semaphores.size() );
		submit_info.pWaitSemaphores      = wait_semaphores.data();
		submit_info.pWaitDstStageMask    = wait_stages.data();
		submit_info.signalSemaphoreCount = static_cast< uint32_t >( signal_semaphores.size() );
		submit_info.pSignalSemaphores    = signal_semaphores.data();

		if( vkQueueSubmit( m_graphics_queue, 1, &submit_info, render_fence ) != VK_SUCCESS )
		{
			DF_LOG_WARNING( "Failed to submit render queue" );
			return;
		}

		const std::vector swap_chains = { m_swapchain };

		VkPresentInfoKHR present_info   = helper::init::presentInfo();
		present_info.waitSemaphoreCount = static_cast< uint32_t >( signal_semaphores.size() );
		present_info.pWaitSemaphores    = signal_semaphores.data();
		present_info.swapchainCount     = static_cast< uint32_t >( swap_chains.size() );
		present_info.pSwapchains        = swap_chains.data();
		present_info.pImageIndices      = &swapchain_image_index;

		result = vkQueuePresentKHR( m_present_queue, &present_info );

		if( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window_resized )
		{
			m_window_resized = false;
			resize();
		}
		else if( result != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Queue present failed" );
			return;
		}

		m_frame_number = ++m_frame_number % frame_overlap;
	}

	void cRenderer_vulkan::beginRendering( const int /*_buffers*/, const cColor& _color )
	{
		ZoneScoped;

		const VkImageSubresourceRange clear_range = helper::init::imageSubresourceRange( VK_IMAGE_ASPECT_COLOR_BIT );
		const VkClearColorValue       clear_value = {
            {_color.r, _color.g, _color.b, _color.a}
		};

		vkCmdClearColorImage( getCurrentFrame().command_buffer, m_render_image.image, VK_IMAGE_LAYOUT_GENERAL, &clear_value, 1, &clear_range );

		helper::util::transitionImage( current_render_command_buffer, m_render_image.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
		helper::util::transitionImage( current_render_command_buffer, m_depth_image.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL );

		const VkRenderingAttachmentInfo color_attachment = helper::init::attachmentInfo( m_render_image.image_view, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
		const VkRenderingAttachmentInfo depth_attachment = helper::init::attachmentInfo( m_depth_image.image_view, nullptr, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL );
		const VkRenderingInfo           rendering_info   = helper::init::renderingInfo( m_render_extent, color_attachment, depth_attachment );
		vkCmdBeginRendering( current_render_command_buffer, &rendering_info );
	}

	void cRenderer_vulkan::endRendering()
	{
		ZoneScoped;

		vkCmdEndRendering( current_render_command_buffer );
	}

	void cRenderer_vulkan::immediateSubmit( std::function< void( VkCommandBuffer ) >&& _function ) const
	{
		ZoneScoped;

		vkResetFences( logical_device, 1, &m_submit_context.fence );
		vkResetCommandBuffer( m_submit_context.command_buffer, 0 );

		VkCommandBuffer                command_buffer            = m_submit_context.command_buffer;
		const VkCommandBufferBeginInfo command_buffer_begin_info = helper::init::commandBufferBeginInfo( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );

		if( vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to begin command buffer" );
			return;
		}

		_function( command_buffer );

		if( vkEndCommandBuffer( command_buffer ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to end command buffer" );
			return;
		}

		const VkSubmitInfo submit_info = helper::init::submitInfo( command_buffer );
		if( vkQueueSubmit( m_graphics_queue, 1, &submit_info, m_submit_context.fence ) != VK_SUCCESS )
		{
			DF_LOG_WARNING( "Failed to submit render queue" );
			return;
		}

		vkWaitForFences( logical_device, 1, &m_submit_context.fence, true, UINT64_MAX );
	}

	void cRenderer_vulkan::createSwapchain( const uint32_t _width, const uint32_t _height )
	{
		ZoneScoped;

		vkb::SwapchainBuilder builder( physical_device, logical_device, m_surface );
		builder.use_default_format_selection();
		builder.set_desired_format( VkSurfaceFormatKHR{ .format = VK_FORMAT_B8G8R8A8_UNORM, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR } );
		builder.set_desired_present_mode( VK_PRESENT_MODE_MAILBOX_KHR );
		builder.set_desired_extent( _width, _height );
		builder.add_image_usage_flags( VK_IMAGE_USAGE_TRANSFER_DST_BIT );

		vkb::Swapchain swapchain = builder.build().value();

		m_swapchain             = swapchain.swapchain;
		m_swapchain_extent      = swapchain.extent;
		m_swapchain_format      = swapchain.image_format;
		m_swapchain_images      = swapchain.get_images().value();
		m_swapchain_image_views = swapchain.get_image_views().value();

		m_render_extent = {
			.width  = m_swapchain_extent.width,
			.height = m_swapchain_extent.height,
		};
		m_depth_image = {
			.extent = {.width = m_render_extent.width, .height = m_render_extent.height, .depth = 1},
			.format = VK_FORMAT_D32_SFLOAT,
		};
		m_render_image = {
			.extent = {.width = m_render_extent.width, .height = m_render_extent.height, .depth = 1},
			.format = VK_FORMAT_R16G16B16A16_SFLOAT,
		};

		constexpr VkImageUsageFlags depth_usage_flags       = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		const VkImageCreateInfo     depth_image_create_info = helper::init::imageCreateInfo( m_depth_image.format, depth_usage_flags, m_depth_image.extent );

		constexpr VkImageUsageFlags render_usage_flags       = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		const VkImageCreateInfo     render_image_create_info = helper::init::imageCreateInfo( m_render_image.format, render_usage_flags, m_render_image.extent );

		constexpr VmaAllocationCreateInfo allocation_create_info{
			.usage         = VMA_MEMORY_USAGE_GPU_ONLY,
			.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		};

		vmaCreateImage( memory_allocator, &depth_image_create_info, &allocation_create_info, &m_depth_image.image, &m_depth_image.allocation, nullptr );
		vmaCreateImage( memory_allocator, &render_image_create_info, &allocation_create_info, &m_render_image.image, &m_render_image.allocation, nullptr );

		VkImageViewCreateInfo depth_image_view_create_info  = helper::init::imageViewCreateInfo( m_depth_image.format, m_depth_image.image, VK_IMAGE_ASPECT_DEPTH_BIT );
		VkImageViewCreateInfo render_image_view_create_info = helper::init::imageViewCreateInfo( m_render_image.format, m_render_image.image, VK_IMAGE_ASPECT_COLOR_BIT );

		vkCreateImageView( logical_device, &depth_image_view_create_info, nullptr, &m_depth_image.image_view );
		vkCreateImageView( logical_device, &render_image_view_create_info, nullptr, &m_render_image.image_view );
	}

	void cRenderer_vulkan::createFrameDatas()
	{
		ZoneScoped;

		const VkCommandPoolCreateInfo command_pool_create_info = helper::init::commandPoolCreateInfo( m_graphics_queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );
		const VkSemaphoreCreateInfo   semaphore_create_info    = helper::init::semaphoreCreateInfo();
		const VkFenceCreateInfo       fence_create_info        = helper::init::fenceCreateInfo( VK_FENCE_CREATE_SIGNALED_BIT );

		for( sFrameData& frame_data: m_frames )
		{
			vkCreateCommandPool( logical_device, &command_pool_create_info, nullptr, &frame_data.command_pool );

			VkCommandBufferAllocateInfo command_buffer_allocate_info = helper::init::commandBufferAllocateInfo( frame_data.command_pool );
			vkAllocateCommandBuffers( logical_device, &command_buffer_allocate_info, &frame_data.command_buffer );

			vkCreateSemaphore( logical_device, &semaphore_create_info, nullptr, &frame_data.swapchain_semaphore );
			vkCreateSemaphore( logical_device, &semaphore_create_info, nullptr, &frame_data.render_semaphore );
			vkCreateFence( logical_device, &fence_create_info, nullptr, &frame_data.render_fence );

			frame_data.vertex_scene_buffer = helper::util::createBuffer( sizeof( sVertexSceneConstants ), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU );

			std::vector< sDescriptorAllocator_vulkan::sPoolSizeRatio > frame_sizes{
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,           3},
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         3},
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         3},
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3},
			};

			frame_data.descriptors.create( logical_device, 1000, frame_sizes );
		}
	}

	void cRenderer_vulkan::createMemoryAllocator()
	{
		ZoneScoped;

		const VmaAllocatorCreateInfo create_info{
			.flags            = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
			.physicalDevice   = physical_device,
			.device           = logical_device,
			.pVulkanFunctions = nullptr,
			.instance         = m_instance,
			.vulkanApiVersion = VK_API_VERSION_1_3,
		};

		if( vmaCreateAllocator( &create_info, &memory_allocator ) != VK_SUCCESS )
			DF_LOG_ERROR( "Failed to create memory allocator" );
		else
			DF_LOG_MESSAGE( "Created memory allocator" );
	}

	void cRenderer_vulkan::createSubmitContext()
	{
		ZoneScoped;

		const VkFenceCreateInfo fence_create_info = helper::init::fenceCreateInfo();
		if( vkCreateFence( logical_device, &fence_create_info, nullptr, &m_submit_context.fence ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create sync objects" );
			return;
		}

		const VkCommandPoolCreateInfo create_info = helper::init::commandPoolCreateInfo( m_graphics_queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );
		if( vkCreateCommandPool( logical_device, &create_info, nullptr, &m_submit_context.command_pool ) != VK_SUCCESS )
		{
			DF_LOG_ERROR( "Failed to create command pool" );
			return;
		}

		const VkCommandBufferAllocateInfo allocate_info = helper::init::commandBufferAllocateInfo( m_submit_context.command_pool );
		if( vkAllocateCommandBuffers( logical_device, &allocate_info, &m_submit_context.command_buffer ) != VK_SUCCESS )
			DF_LOG_ERROR( "Failed to allocate command buffer" );
		else
			DF_LOG_MESSAGE( "Created submit context" );
	}

	void cRenderer_vulkan::resize()
	{
		ZoneScoped;

		int width = 0, height = 0;
		while( width == 0 || height == 0 )
		{
			glfwGetFramebufferSize( m_window, &width, &height );
			glfwWaitEvents();
		}

		vkDeviceWaitIdle( logical_device );
		m_window_size.x = width;
		m_window_size.y = height;

		for( const VkImageView& image_view: m_swapchain_image_views )
			vkDestroyImageView( logical_device, image_view, nullptr );

		vkDestroySwapchainKHR( logical_device, m_swapchain, nullptr );
		createSwapchain( width, height );

		DF_LOG_MESSAGE( fmt::format( "Resized window [{}, {}]", m_window_size.x, m_window_size.y ) );
	}

	void cRenderer_vulkan::framebufferSizeCallback( GLFWwindow* _window, int /*_width*/, int /*_height*/ )
	{
		ZoneScoped;

		cRenderer_vulkan* renderer = static_cast< cRenderer_vulkan* >( glfwGetWindowUserPointer( _window ) );
		renderer->m_window_resized = true;
	}

	VkBool32 cRenderer_vulkan::debugMessageCallback( const VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	                                                 const VkDebugUtilsMessageTypeFlagsEXT        _message_type,
	                                                 const VkDebugUtilsMessengerCallbackDataEXT*  _callback_data,
	                                                 void* /*_user_data*/ )
	{
		ZoneScoped;

		std::string type = "None";
		if( _message_type >= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT )
			type = "Performance";
		else if( _message_type >= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT )
			type = "Validation";
		else if( _message_type >= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT )
			type = "General";

		if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
		{
			DF_LOG_ERROR( fmt::format( "Vulkan, "
			                           "Type: {}, "
			                           "Severity: Error, "
			                           "Message: {}",
			                           type,
			                           _callback_data->pMessage ) );
		}
		else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
		{
			DF_LOG_WARNING( fmt::format( "Vulkan, "
			                             "Type: {}, "
			                             "Severity: Warning, "
			                             "Message: {}",
			                             type,
			                             _callback_data->pMessage ) );
		}
		else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT )
		{
			DF_LOG_MESSAGE( fmt::format( "Vulkan, "
			                             "Type: {}, "
			                             "Severity: Info, "
			                             "Message: {}",
			                             type,
			                             _callback_data->pMessage ) );
		}
		else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT )
		{
			DF_LOG_MESSAGE( fmt::format( "Vulkan, "
			                             "Type: {}, "
			                             "Severity: Verbose, "
			                             "Message: {}",
			                             type,
			                             _callback_data->pMessage ) );
		}
		else
		{
			DF_LOG_MESSAGE( fmt::format( "Vulkan, "
			                             "Type: {}, "
			                             "Severity: None, "
			                             "Message: {}",
			                             type,
			                             _callback_data->pMessage ) );
		}

		return false;
	}
}
