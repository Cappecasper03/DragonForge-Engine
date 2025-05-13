#include "cRenderer_vulkan.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>

#include "assets/cQuad_vulkan.h"
#include "callbacks/cDefaultQuad_vulkan.h"
#include "cFramebuffer_vulkan.h"
#include "descriptor/sDescriptorLayoutBuilder_vulkan.h"
#include "descriptor/sDescriptorWriter_vulkan.h"
#include "engine/core/math/math.h"
#include "engine/core/utils/cTransform.h"
#include "engine/graphics/api/iFramebuffer.h"
#include "engine/graphics/assets/iQuad.h"
#include "engine/graphics/callback/cRenderCallback.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/lights/sLight.h"
#include "engine/graphics/types/sSceneUniforms.h"
#include "engine/graphics/window/WindowTypes.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cLightManager.h"
#include "engine/profiling/ProfilingMacros_vulkan.h"
#include "types/Helper_vulkan.h"
#include "window/cWindow_vulkan.h"

namespace df::vulkan
{
	cRenderer_vulkan::cRenderer_vulkan( const std::string& _window_name )
		: m_frames_in_flight( 1 )
		, m_frame_number( 0 )
		, m_frame_data( m_frames_in_flight )
		, m_begin_deferred( true )
		, m_last_camera_type( cCamera::kNone )
	{
		DF_ProfilingScopeCpu;

		m_window = new cWindow_vulkan();

		m_window->create( _window_name, window::kVulkan | window::kResizable );

		VULKAN_HPP_DEFAULT_DISPATCHER.init();

		uint32_t           extension_count;
		char const* const* required_extensions = SDL_Vulkan_GetInstanceExtensions( &extension_count );

		std::vector< const char* > instance_extension_names;
		instance_extension_names.reserve( extension_count );

		for( uint32_t i = 0; i < extension_count; ++i )
			instance_extension_names.push_back( required_extensions[ i ] );

		std::vector< const char* > instance_layer_names;
		const void*                debug_info_pointer = nullptr;
#ifdef DF_Debug
	#ifdef DF_Windows
		instance_layer_names.push_back( "VK_LAYER_KHRONOS_validation" );
	#endif

		instance_extension_names.push_back( vk::EXTDebugUtilsExtensionName );

		vk::DebugUtilsMessageSeverityFlagsEXT severity_flags  = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
		severity_flags                                       |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
		severity_flags                                       |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;

		vk::DebugUtilsMessageTypeFlagsEXT message_type_flags  = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral;
		message_type_flags                                   |= vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
		message_type_flags                                   |= vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

		const vk::DebugUtilsMessengerCreateInfoEXT debug_create_info( vk::DebugUtilsMessengerCreateFlagsEXT(),
		                                                              severity_flags,
		                                                              message_type_flags,
		                                                              &cRenderer_vulkan::debugMessageCallback );

		debug_info_pointer = reinterpret_cast< const void* >( &debug_create_info );
#endif

		const vk::ApplicationInfo    application_info( _window_name.data(), 0, "DragonForge", 0, vk::ApiVersion14 );
		const vk::InstanceCreateInfo instance_create_info( vk::InstanceCreateFlags(), &application_info, instance_layer_names, instance_extension_names, debug_info_pointer );
		m_instance = createInstanceUnique( instance_create_info ).value;

		VULKAN_HPP_DEFAULT_DISPATCHER.init( m_instance.get() );

#ifdef DF_Debug
		m_debug_messenger = m_instance->createDebugUtilsMessengerEXTUnique( debug_create_info ).value;
#endif

		VkSurfaceKHR temp_surface{};
		SDL_Vulkan_CreateSurface( m_window->getWindow(), m_instance.get(), nullptr, &temp_surface );
		m_surface = vk::UniqueSurfaceKHR( temp_surface, m_instance.get() );

		m_physical_device = m_instance->enumeratePhysicalDevices().value.front();

		const std::vector< vk::QueueFamilyProperties > queue_family_properties = m_physical_device.getQueueFamilyProperties();

		const auto it = std::find_if( queue_family_properties.begin(),
		                              queue_family_properties.end(),
		                              []( vk::QueueFamilyProperties const& _properties ) { return _properties.queueFlags & vk::QueueFlagBits::eGraphics; } );

		m_graphics_queue_family = static_cast< uint32_t >( std::distance( queue_family_properties.begin(), it ) );

		std::vector device_extension_names = { vk::KHRSwapchainExtensionName };
#ifdef DF_Profiling
		device_extension_names.push_back( vk::KHRCalibratedTimestampsExtensionName );
#endif

		vk::PhysicalDeviceSynchronization2Features    synchronization2_features( true );
		vk::PhysicalDeviceBufferDeviceAddressFeatures buffer_device_address_features( true, false, false, &synchronization2_features );
		vk::PhysicalDeviceDynamicRenderingFeatures    dynamic_rendering_features( true, &buffer_device_address_features );

		constexpr float           queue_priority = 0;
		vk::DeviceQueueCreateInfo device_queue_create_info( vk::DeviceQueueCreateFlags(), m_graphics_queue_family, 1, &queue_priority );
		m_logical_device = m_physical_device
		                       .createDeviceUnique(
								   vk::DeviceCreateInfo( vk::DeviceCreateFlags(), device_queue_create_info, {}, device_extension_names, {}, &dynamic_rendering_features ) )
		                       .value;
		m_graphics_queue = m_logical_device->getQueue( m_graphics_queue_family, 0 );

		VULKAN_HPP_DEFAULT_DISPATCHER.init( m_logical_device.get() );

		createMemoryAllocator();
		createSwapchain( m_window->getSize().x(), m_window->getSize().y() );

		m_get_instance_proc_addr = reinterpret_cast< PFN_vkGetInstanceProcAddr >( m_instance->getProcAddr( "vkGetInstanceProcAddr" ) );
		m_get_device_proc_addr   = reinterpret_cast< PFN_vkGetDeviceProcAddr >( m_instance->getProcAddr( "vkGetDeviceProcAddr" ) );

		for( sFrameData_vulkan& frame_data: m_frame_data )
			frame_data.create( this );

		m_submit_context.create( this );

		m_sampler_linear  = m_logical_device->createSamplerUnique( vk::SamplerCreateInfo( vk::SamplerCreateFlags(), vk::Filter::eLinear, vk::Filter::eLinear ) ).value;
		m_sampler_nearest = m_logical_device->createSamplerUnique( vk::SamplerCreateInfo( vk::SamplerCreateFlags(), vk::Filter::eNearest, vk::Filter::eNearest ) ).value;

		DF_LogMessage( "Initialized renderer" );
	}

	cRenderer_vulkan::~cRenderer_vulkan()
	{
		DF_ProfilingScopeCpu;

		if( m_logical_device->waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );

		if( cRenderer::isDeferred() )
		{
			delete m_deferred_framebuffer;
			delete m_deferred_screen_quad->render_callback;
			delete m_deferred_screen_quad;
			m_deferred_layout.reset();
		}

		if( ImGui::GetCurrentContext() )
		{
			ImGui_ImplVulkan_Shutdown();
			m_imgui_descriptor_pool.reset();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();
		}

		m_sampler_nearest.reset();
		m_sampler_linear.reset();

		m_submit_context.destroy();

		for( sFrameData_vulkan& frame_data: m_frame_data )
			frame_data.destroy();

		helper::util::destroyImage( m_render_image );
		helper::util::destroyImage( m_depth_image );

		for( vk::UniqueImageView& swapchain_image_view: m_swapchain_image_views )
			swapchain_image_view.reset();

		m_swapchain.reset();

		memory_allocator.reset();

		m_logical_device.reset();

		m_surface.reset();

		m_debug_messenger.reset();

		m_instance.reset();

		delete m_window;
	}

	void cRenderer_vulkan::render()
	{
		DF_ProfilingScopeCpu;

		if( m_window_minimized )
			return;

		sFrameData_vulkan&    frame_data     = getCurrentFrame();
		const cCommandBuffer& command_buffer = frame_data.command_buffer;

		vk::Result result = m_logical_device->waitForFences( 1, &frame_data.render_fence.get(), true, std::numeric_limits< uint64_t >::max() );
		frame_data.dynamic_descriptors.clear();
		if( result != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for fences" );

		uint32_t swapchain_image_index;
		result = m_logical_device->acquireNextImageKHR( m_swapchain.get(),
		                                                std::numeric_limits< uint64_t >::max(),
		                                                frame_data.swapchain_semaphore.get(),
		                                                nullptr,
		                                                &swapchain_image_index );

		if( result == vk::Result::eErrorOutOfDateKHR )
		{
			resize();
			return;
		}

		if( result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR )
		{
			DF_LogError( "Failed to acquire next image" );
			return;
		}

		result = m_logical_device->resetFences( 1, &frame_data.render_fence.get() );
		if( result != vk::Result::eSuccess )
			DF_LogError( "Failed to reset fences" );

		m_logical_device->resetCommandPool( frame_data.command_pool.get() );

		command_buffer.begin();

		{
			DF_ProfilingScopeCpu;
			DF_ProfilingScopeGpu( frame_data.profiling_context, command_buffer.get() );

			helper::util::transitionImage( command_buffer.get(), m_render_image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral );

			{
				m_last_camera_type = cCamera::kNone;

				const sAllocatedBuffer_vulkan& buffer = frame_data.fragment_scene_uniform_buffer;
				const vk::DescriptorSet&       set    = frame_data.fragment_scene_descriptor_set;

				const std::vector< sLight >& lights      = cLightManager::getLights();
				const unsigned               light_count = static_cast< unsigned >( lights.size() );

				const size_t     current_lights_size = sizeof( sLight ) * lights.size();
				constexpr size_t full_lights_size    = sizeof( sLight ) * cLightManager::m_max_lights;
				constexpr size_t total_size          = sizeof( sLight ) * cLightManager::m_max_lights + sizeof( light_count );

				helper::util::setBufferData( lights.data(), current_lights_size, 0, buffer );
				helper::util::setBufferData( &light_count, sizeof( light_count ), full_lights_size, buffer );

				sDescriptorWriter_vulkan writer_scene;
				writer_scene.writeBuffer( 0, buffer.buffer.get(), total_size, 0, vk::DescriptorType::eUniformBuffer );
				writer_scene.updateSet( set );
			}

			if( cRenderer::isDeferred() )
				renderDeferred( command_buffer.get() );
			else
			{
				cEventManager::invoke( event::render_3d );
				cEventManager::invoke( event::render_2d );
			}

			helper::util::transitionImage( command_buffer.get(), m_render_image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferSrcOptimal );
			helper::util::transitionImage( command_buffer.get(), m_swapchain_images[ swapchain_image_index ], vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );

			helper::util::copyImageToImage( command_buffer.get(), m_render_image.image.get(), m_swapchain_images[ swapchain_image_index ], m_render_extent, m_swapchain_extent );

			if( ImGui::GetCurrentContext() )
			{
				ImGui_ImplVulkan_NewFrame();
				ImGui_ImplSDL3_NewFrame();
				ImGui::NewFrame();
				cEventManager::invoke( event::imgui );
				ImGui::Render();

				const vk::RenderingAttachmentInfo color_attachment = helper::init::attachmentInfo( m_swapchain_image_views[ swapchain_image_index ].get(),
				                                                                                   nullptr,
				                                                                                   vk::ImageLayout::eColorAttachmentOptimal );

				command_buffer.beginRendering( m_swapchain_extent, &color_attachment );

				ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), command_buffer.get() );
				command_buffer.endRendering();
			}

			helper::util::transitionImage( command_buffer.get(),
			                               m_swapchain_images[ swapchain_image_index ],
			                               vk::ImageLayout::eTransferDstOptimal,
			                               vk::ImageLayout::ePresentSrcKHR );

			DF_ProfilingCollectGpu( frame_data.profiling_context, command_buffer.get() );
		}

		command_buffer.end();

		const vk::CommandBufferSubmitInfo command_buffer_submit_info   = helper::init::commandBufferSubmitInfo( command_buffer.get() );
		const vk::SemaphoreSubmitInfo     wait_semaphore_submit_info   = helper::init::semaphoreSubmitInfo( vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                                                                                                      frame_data.swapchain_semaphore.get() );
		const vk::SemaphoreSubmitInfo     signal_semaphore_submit_info = helper::init::semaphoreSubmitInfo( vk::PipelineStageFlagBits2::eAllGraphics,
                                                                                                        frame_data.render_semaphore.get() );
		const vk::SubmitInfo2             submit_info = helper::init::submitInfo( &command_buffer_submit_info, &signal_semaphore_submit_info, &wait_semaphore_submit_info );

		if( m_graphics_queue.submit2( 1, &submit_info, frame_data.render_fence.get() ) != vk::Result::eSuccess )
		{
			DF_LogWarning( "Failed to submit render queue" );
			return;
		}

		const vk::PresentInfoKHR present_info = helper::init::presentInfo( &frame_data.render_semaphore.get(), &m_swapchain.get(), &swapchain_image_index );

		result = m_graphics_queue.presentKHR( &present_info );
		if( result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_window_resized )
			resize();
		else if( result != vk::Result::eSuccess )
		{
			DF_LogError( "Queue present failed" );
			return;
		}

		m_frame_number++;
	}

	void cRenderer_vulkan::beginRendering( const int _clear_buffers, const cColor& _color )
	{
		DF_ProfilingScopeCpu;
		const sFrameData_vulkan& frame_data = getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const bool color = _clear_buffers & cCamera::eClearBuffer::kColor;
		const bool depth = _clear_buffers & cCamera::eClearBuffer::kDepth;

		const cCommandBuffer&    command_buffer = frame_data.command_buffer;
		const vk::ClearValue     clear_color_value( vk::ClearColorValue( _color.r, _color.g, _color.b, _color.a ) );
		constexpr vk::ClearValue clear_depth_stencil_value( vk::ClearDepthStencilValue( 1 ) );

		const vk::RenderingAttachmentInfo depth_attachment = helper::init::attachmentInfo( m_depth_image.image_view.get(),
		                                                                                   depth ? &clear_depth_stencil_value : nullptr,
		                                                                                   vk::ImageLayout::eDepthAttachmentOptimal );

		if( cRenderer::isDeferred() && m_begin_deferred )
		{
			const cFramebuffer_vulkan*                   framebuffer        = reinterpret_cast< cFramebuffer_vulkan* >( m_deferred_framebuffer );
			const std::vector< sAllocatedImage_vulkan >& framebuffer_images = framebuffer->getCurrentFrameImages( getCurrentFrameIndex() );

			std::vector< vk::RenderingAttachmentInfo > color_attachments;
			color_attachments.reserve( framebuffer_images.size() );

			for( const sAllocatedImage_vulkan& framebuffer_image: framebuffer_images )
			{
				color_attachments.push_back(
					helper::init::attachmentInfo( framebuffer_image.image_view.get(), color ? &clear_color_value : nullptr, vk::ImageLayout::eColorAttachmentOptimal ) );
			}

			command_buffer.beginRendering( m_render_extent, color_attachments, &depth_attachment );

			{
				const cCamera* camera = cCameraManager::getInstance()->current;
				if( m_last_camera_type == camera->type )
					return;

				m_last_camera_type                    = camera->type;
				const sAllocatedBuffer_vulkan& buffer = frame_data.getVertexSceneBuffer();
				const vk::DescriptorSet&       set    = frame_data.getVertexDescriptorSet();

				const sVertexSceneUniforms uniforms{
					.view_projection = camera->view_projection,
				};

				helper::util::setBufferData( &uniforms, sizeof( uniforms ), 0, buffer );

				sDescriptorWriter_vulkan writer_scene;
				writer_scene.writeBuffer( 0, buffer.buffer.get(), sizeof( uniforms ), 0, vk::DescriptorType::eUniformBuffer );
				writer_scene.updateSet( set );
			}
		}
		else
		{
			const vk::RenderingAttachmentInfo color_attachment = helper::init::attachmentInfo( m_render_image.image_view.get(),
			                                                                                   color ? &clear_color_value : nullptr,
			                                                                                   vk::ImageLayout::eColorAttachmentOptimal );

			command_buffer.beginRendering( m_render_extent, &color_attachment, &depth_attachment );

			{
				const cCamera* camera = cCameraManager::getInstance()->current;
				if( m_last_camera_type == camera->type )
					return;

				m_last_camera_type                    = camera->type;
				const sAllocatedBuffer_vulkan& buffer = frame_data.getVertexSceneBuffer();
				const vk::DescriptorSet&       set    = frame_data.getVertexDescriptorSet();

				const sVertexSceneUniforms uniforms{
					.view_projection = camera->view_projection,
				};

				helper::util::setBufferData( &uniforms, sizeof( uniforms ), 0, buffer );

				sDescriptorWriter_vulkan writer_scene;
				writer_scene.writeBuffer( 0, buffer.buffer.get(), sizeof( uniforms ), 0, vk::DescriptorType::eUniformBuffer );
				writer_scene.updateSet( set );
			}
		}
	}

	void cRenderer_vulkan::endRendering()
	{
		DF_ProfilingScopeCpu;
		const sFrameData_vulkan& frame_data = getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );

		const cCommandBuffer& command_buffer = frame_data.command_buffer;
		command_buffer.endRendering();
	}

	void cRenderer_vulkan::immediateSubmit( const std::function< void( vk::CommandBuffer ) >& _function ) const
	{
		DF_ProfilingScopeCpu;

		if( m_logical_device->resetFences( 1, &m_submit_context.fence.get() ) != vk::Result::eSuccess )
			DF_LogError( "Failed to reset fences" );

		if( m_logical_device->resetCommandPool( m_submit_context.command_pool.get() ) != vk::Result::eSuccess )
			DF_LogError( "Failed to reset command pool" );

		const cCommandBuffer& command_buffer = m_submit_context.command_buffer;

		command_buffer.begin();

		{
			DF_ProfilingScopeGpu( m_submit_context.tracy_context, m_submit_context.command_buffer.get() );

			_function( command_buffer.get() );
		}

		DF_ProfilingCollectGpu( m_submit_context.tracy_context, m_submit_context.command_buffer.get() );

		command_buffer.end();

		const vk::CommandBufferSubmitInfo buffer_submit_info = helper::init::commandBufferSubmitInfo( command_buffer.get() );
		const vk::SubmitInfo2             submit_info        = helper::init::submitInfo( &buffer_submit_info );

		if( m_graphics_queue.submit2( 1, &submit_info, m_submit_context.fence.get() ) != vk::Result::eSuccess )
		{
			DF_LogWarning( "Failed to submit render queue" );
			return;
		}

		if( m_logical_device->waitForFences( 1, &m_submit_context.fence.get(), true, std::numeric_limits< uint64_t >::max() ) != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for fences" );
	}

	void cRenderer_vulkan::setViewport()
	{
		const vk::Viewport viewport( 0, 0, static_cast< float >( m_render_extent.width ), static_cast< float >( m_render_extent.height ), 0, 1 );
		getCurrentFrame().command_buffer.setViewport( 0, 1, viewport );
	}

	void cRenderer_vulkan::setScissor()
	{
		const vk::Rect2D scissor( vk::Offset2D(), vk::Extent2D( m_render_extent.width, m_render_extent.height ) );
		getCurrentFrame().command_buffer.setScissor( 0, 1, scissor );
	}

	void cRenderer_vulkan::setViewportScissor()
	{
		DF_ProfilingScopeCpu;

		setViewport();
		setScissor();
	}

	void cRenderer_vulkan::initializeImGui()
	{
		DF_ProfilingScopeCpu;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io     = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui_ImplSDL3_InitForVulkan( m_window->getWindow() );

		const std::vector pool_sizes = {
			vk::DescriptorPoolSize( vk::DescriptorType::eSampler, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eCombinedImageSampler, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eSampledImage, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eStorageImage, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eUniformTexelBuffer, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eStorageTexelBuffer, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eUniformBuffer, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eStorageBuffer, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eUniformBufferDynamic, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eStorageBufferDynamic, 1000 ),
			vk::DescriptorPoolSize( vk::DescriptorType::eInputAttachment, 1000 ),
		};

		const vk::DescriptorPoolCreateInfo create_info( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, static_cast< uint32_t >( 1000 ), pool_sizes );

		m_imgui_descriptor_pool = m_logical_device->createDescriptorPoolUnique( create_info ).value;

		ImGui_ImplVulkan_InitInfo init_info{
			.Instance                    = m_instance.get(),
			.PhysicalDevice              = m_physical_device,
			.Device                      = m_logical_device.get(),
			.QueueFamily                 = m_graphics_queue_family,
			.Queue                       = m_graphics_queue,
			.DescriptorPool              = m_imgui_descriptor_pool.get(),
			.MinImageCount               = 3,
			.ImageCount                  = 3,
			.MSAASamples                 = static_cast< VkSampleCountFlagBits >( vk::SampleCountFlagBits::e1 ),
			.UseDynamicRendering         = true,
			.PipelineRenderingCreateInfo = {
				.sType					 = static_cast< VkStructureType >( vk::StructureType::ePipelineRenderingCreateInfo ),
				.colorAttachmentCount	 = 1,
				.pColorAttachmentFormats = reinterpret_cast< const VkFormat* >( &m_swapchain_format ),
			},
		};
		ImGui_ImplVulkan_Init( &init_info );
	}

	void cRenderer_vulkan::renderDeferred( const vk::CommandBuffer& _command_buffer )
	{
		DF_ProfilingScopeCpu;
#ifdef DF_Profiling
		const sFrameData_vulkan& frame_data = getCurrentFrame();
		DF_ProfilingScopeGpu( frame_data.profiling_context, frame_data.command_buffer.get() );
#endif

		const cFramebuffer_vulkan*                   framebuffer     = reinterpret_cast< cFramebuffer_vulkan* >( m_deferred_framebuffer );
		const std::vector< sAllocatedImage_vulkan >& deferred_images = framebuffer->getCurrentFrameImages( getCurrentFrameIndex() );

		for( const sAllocatedImage_vulkan& image: deferred_images )
			helper::util::transitionImage( _command_buffer, image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral );

		m_begin_deferred = true;
		cEventManager::invoke( event::render_3d );
		cEventManager::invoke( event::render_2d );

		for( const sAllocatedImage_vulkan& image: deferred_images )
			helper::util::transitionImage( _command_buffer, image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal );

		m_begin_deferred = false;
		cCamera* camera  = cCameraManager::get( "default_2d" );
		camera->beginRender( cCamera::kDepth );
		m_deferred_screen_quad->render();
		camera->endRender();
	}

	void cRenderer_vulkan::initializeDeferred()
	{
		DF_ProfilingScopeCpu;

		sPipelineCreateInfo_vulkan pipeline_create_info{ .name = "deferred_quad_final" };

		pipeline_create_info.vertex_input_binding.emplace_back( 0, static_cast< uint32_t >( sizeof( cQuad_vulkan::sVertex ) ), vk::VertexInputRate::eVertex );

		pipeline_create_info.vertex_input_attribute.emplace_back( 0,
		                                                          0,
		                                                          vk::Format::eR32G32B32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( cQuad_vulkan::sVertex, cQuad_vulkan::sVertex::position ) ) );
		pipeline_create_info.vertex_input_attribute.emplace_back( 1,
		                                                          0,
		                                                          vk::Format::eR32G32Sfloat,
		                                                          static_cast< uint32_t >( offsetof( cQuad_vulkan::sVertex, cQuad_vulkan::sVertex::tex_coord ) ) );

		pipeline_create_info.push_constant_ranges.emplace_back( vk::ShaderStageFlagBits::eVertex, 0, static_cast< uint32_t >( sizeof( iQuad::sPushConstants ) ) );

		sDescriptorLayoutBuilder_vulkan descriptor_layout_builder{};
		descriptor_layout_builder.addBinding( 0, vk::DescriptorType::eSampler );
		descriptor_layout_builder.addBinding( 1, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 2, vk::DescriptorType::eSampledImage );
		descriptor_layout_builder.addBinding( 3, vk::DescriptorType::eSampledImage );
		m_deferred_layout = descriptor_layout_builder.build( vk::ShaderStageFlagBits::eFragment );

		pipeline_create_info.descriptor_layouts.push_back( sFrameData_vulkan::s_vertex_scene_descriptor_set_layout.get() );
		pipeline_create_info.descriptor_layouts.push_back( m_deferred_layout.get() );

		pipeline_create_info.setShaders( helper::util::createShaderModule( "deferred_quad_final.vert" ), helper::util::createShaderModule( "deferred_quad_final.frag" ) );
		pipeline_create_info.setInputTopology( vk::PrimitiveTopology::eTriangleList );
		pipeline_create_info.setPolygonMode( vk::PolygonMode::eFill );
		pipeline_create_info.setCullMode( vk::CullModeFlagBits::eFront, vk::FrontFace::eClockwise );
		pipeline_create_info.setColorFormat( getRenderColorFormat() );
		pipeline_create_info.setDepthFormat( getRenderDepthFormat() );
		pipeline_create_info.setMultisamplingNone();
		pipeline_create_info.enableDepthTest( true, vk::CompareOp::eLessOrEqual );
		pipeline_create_info.disableBlending();

		sDescriptorWriter_vulkan writer_scene;
		for( sFrameData_vulkan& frame_data: getFrameData() )
			m_descriptors.push_back( frame_data.static_descriptors.allocate( m_deferred_layout.get() ) );

		m_deferred_screen_quad = new cQuad_vulkan( "deferred", cVector3f( m_window->getSize() / 2, 0 ), m_window->getSize(), color::white, false );
		cMatrix4f& transform   = m_deferred_screen_quad->transform->local;
		transform.rotate( math::radians( 180.f ), cVector3f( 0.f, 0.f, 1.f ) );
		transform.rotate( math::radians( 180.f ), cVector3f( 0.f, 1.f, 0.f ) );
		m_deferred_screen_quad->transform->update();
		m_deferred_screen_quad->render_callback = new cRenderCallback( "deferred_quad_final", pipeline_create_info, render_callbacks::cDefaultQuad_vulkan::deferredQuadFinal );

		m_deferred_framebuffer = new cFramebuffer_vulkan( "deferred", 3, m_frames_in_flight, m_window->getSize() );
	}

	void cRenderer_vulkan::createSwapchain( const uint32_t _width, const uint32_t _height )
	{
		DF_ProfilingScopeCpu;

		const std::vector< vk::SurfaceFormatKHR > formats              = m_physical_device.getSurfaceFormatsKHR( m_surface.get() ).value;
		const vk::SurfaceCapabilitiesKHR          surface_capabilities = m_physical_device.getSurfaceCapabilitiesKHR( m_surface.get() ).value;

		m_swapchain_format = formats.front().format == vk::Format::eUndefined ? vk::Format::eB8G8R8A8Unorm : formats.front().format;

		if( surface_capabilities.currentExtent.width == std::numeric_limits< uint32_t >::max() )
		{
			m_swapchain_extent.width  = std::clamp( _width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width );
			m_swapchain_extent.height = std::clamp( _height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height );
		}
		else
			m_swapchain_extent = surface_capabilities.currentExtent;

		const std::vector family_indices = { m_graphics_queue_family };

		vk::SwapchainCreateInfoKHR swapchain_create_info( vk::SwapchainCreateFlagsKHR(),
		                                                  m_surface.get(),
		                                                  std::clamp( m_frames_in_flight, surface_capabilities.minImageCount, surface_capabilities.maxImageCount ),
		                                                  m_swapchain_format,
		                                                  vk::ColorSpaceKHR::eSrgbNonlinear,
		                                                  m_swapchain_extent,
		                                                  1,
		                                                  vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
		                                                  vk::SharingMode::eExclusive,
		                                                  family_indices,
		                                                  vk::SurfaceTransformFlagBitsKHR::eIdentity,
		                                                  vk::CompositeAlphaFlagBitsKHR::eOpaque,
		                                                  vk::PresentModeKHR::eMailbox,
		                                                  true );

		m_swapchain = m_logical_device->createSwapchainKHRUnique( swapchain_create_info ).value;

		for( vk::Image image: m_logical_device->getSwapchainImagesKHR( m_swapchain.get() ).value )
		{
			m_swapchain_images.emplace_back( image );

			vk::ImageViewCreateInfo image_view_create_info( vk::ImageViewCreateFlags(),
			                                                image,
			                                                vk::ImageViewType::e2D,
			                                                m_swapchain_format,
			                                                vk::ComponentMapping(),
			                                                vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) );
			m_swapchain_image_views.push_back( m_logical_device->createImageViewUnique( image_view_create_info ).value );
		}

		m_render_extent = vk::Extent2D( m_swapchain_extent.width, m_swapchain_extent.height );
		m_depth_image   = {
			  .extent = vk::Extent3D( m_render_extent.width, m_render_extent.height, 1 ),
			  .format = vk::Format::eD24UnormS8Uint,
		};
		m_render_image = {
			.extent = vk::Extent3D( m_render_extent.width, m_render_extent.height, 1 ),
			.format = vk::Format::eR8G8B8A8Unorm,
		};

		constexpr vk::ImageUsageFlags depth_usage_flags       = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferDst;
		const vk::ImageCreateInfo     depth_image_create_info = helper::init::imageCreateInfo( m_depth_image.format, depth_usage_flags, m_depth_image.extent );

		constexpr vk::ImageUsageFlags render_usage_flags = vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eStorage
		                                                 | vk::ImageUsageFlagBits::eColorAttachment;
		const vk::ImageCreateInfo render_image_create_info = helper::init::imageCreateInfo( m_render_image.format, render_usage_flags, m_render_image.extent );

		constexpr vma::AllocationCreateInfo allocation_create_info( vma::AllocationCreateFlags(), vma::MemoryUsage::eGpuOnly, vk::MemoryPropertyFlagBits::eDeviceLocal );

		std::pair< vma::UniqueImage, vma::UniqueAllocation > depth = memory_allocator->createImageUnique( depth_image_create_info, allocation_create_info ).value;
		m_depth_image.image.swap( depth.first );
		m_depth_image.allocation.swap( depth.second );

		std::pair< vma::UniqueImage, vma::UniqueAllocation > render = memory_allocator->createImageUnique( render_image_create_info, allocation_create_info ).value;
		m_render_image.image.swap( render.first );
		m_render_image.allocation.swap( render.second );

		vk::ImageViewCreateInfo depth_image_view_create_info  = helper::init::imageViewCreateInfo( m_depth_image.format,
                                                                                                  m_depth_image.image.get(),
                                                                                                  vk::ImageAspectFlagBits::eDepth );
		vk::ImageViewCreateInfo render_image_view_create_info = helper::init::imageViewCreateInfo( m_render_image.format,
		                                                                                           m_render_image.image.get(),
		                                                                                           vk::ImageAspectFlagBits::eColor );

		m_depth_image.image_view  = m_logical_device->createImageViewUnique( depth_image_view_create_info ).value;
		m_render_image.image_view = m_logical_device->createImageViewUnique( render_image_view_create_info ).value;
	}

	void cRenderer_vulkan::createMemoryAllocator()
	{
		DF_ProfilingScopeCpu;

		vma::AllocatorCreateInfo create_info( vma::AllocatorCreateFlagBits::eExtMemoryBudget, m_physical_device, m_logical_device.get() );
		create_info.setInstance( m_instance.get() );
		create_info.setVulkanApiVersion( vk::ApiVersion13 );

		memory_allocator = createAllocatorUnique( create_info ).value;
		DF_LogMessage( "Created memory allocator" );
	}

	void cRenderer_vulkan::resize()
	{
		DF_ProfilingScopeCpu;

		int width = 0, height = 0;
		while( width == 0 || height == 0 )
		{
			SDL_GetWindowSize( m_window->getWindow(), &width, &height );
			if( width == 0 || height == 0 )
			{
				SDL_Event event;
				SDL_WaitEvent( &event );

				if( event.type == SDL_EVENT_QUIT )
					return;
			}
		}

		if( m_logical_device->waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );

		m_window->getSize().x() = width;
		m_window->getSize().y() = height;

		helper::util::destroyImage( m_render_image );
		helper::util::destroyImage( m_depth_image );

		for( vk::UniqueImageView& swapchain_image_view: m_swapchain_image_views )
			swapchain_image_view.reset();

		m_swapchain.reset();

		m_swapchain_images.clear();
		m_swapchain_image_views.clear();

		createSwapchain( width, height );

		cEventManager::invoke( event::on_window_resize, width, height );
		m_window_resized = false;
		DF_LogMessage( fmt::format( "Resized window [{}, {}]", m_window->getSize().x(), m_window->getSize().y() ) );
	}

	VkBool32 cRenderer_vulkan::debugMessageCallback( const VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	                                                 const VkDebugUtilsMessageTypeFlagsEXT        _message_type,
	                                                 const VkDebugUtilsMessengerCallbackDataEXT*  _callback_data,
	                                                 void* /*_user_data*/
	)
	{
		DF_ProfilingScopeCpu;

		std::string type = "None";
		if( _message_type >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance ) )
			type = "Performance";
		else if( _message_type >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation ) )
			type = "Validation";
		else if( _message_type >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral ) )
			type = "General";

		std::string message = _callback_data->pMessage;
		std::ranges::replace( message, '\n', ' ' );
		if( _message_severity >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageSeverityFlagBitsEXT::eError ) )
		{
			DF_LogError( fmt::format( "Vulkan, "
			                          "Type: {}, "
			                          "Severity: Error, "
			                          "Message: {}",
			                          type,
			                          message ) );
		}
		else if( _message_severity >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning ) )
		{
			DF_LogWarning( fmt::format( "Vulkan, "
			                            "Type: {}, "
			                            "Severity: Warning, "
			                            "Message: {}",
			                            type,
			                            message ) );
		}
		else if( _message_severity >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo ) )
		{
			DF_LogMessage( fmt::format( "Vulkan, "
			                            "Type: {}, "
			                            "Severity: Info, "
			                            "Message: {}",
			                            type,
			                            message ) );
		}
		else if( _message_severity >= static_cast< VkDebugUtilsMessageTypeFlagsEXT >( vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose ) )
		{
			DF_LogMessage( fmt::format( "Vulkan, "
			                            "Type: {}, "
			                            "Severity: Verbose, "
			                            "Message: {}",
			                            type,
			                            message ) );
		}
		else
		{
			DF_LogMessage( fmt::format( "Vulkan, "
			                            "Type: {}, "
			                            "Severity: None, "
			                            "Message: {}",
			                            type,
			                            message ) );
		}

		return false;
	}
}
