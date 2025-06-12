#pragma once

#include <functional>
#include <vector>
#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/cameras/cCamera.h"
#include "types/sAllocatedImage_vulkan.h"
#include "types/sFrameData_vulkan.h"
#include "types/sSubmitContext_vulkan.h"

namespace df
{
	class iSampler;
}

namespace df::vulkan
{
	class cDeferredRenderer_vulkan;

	class cGraphicsDevice_vulkan : public iGraphicsDevice
	{
	public:
		DF_DeleteCopyAndMove( cGraphicsDevice_vulkan );

		explicit cGraphicsDevice_vulkan( const std::string& _window_name );
		~cGraphicsDevice_vulkan() override;

		void render() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;
		void endRendering() override;

		void immediateSubmit( const std::function< void( vk::CommandBuffer ) >& _function ) const;

		void setViewport();
		void setScissor();
		void setViewportScissor();

		void initialize() override;
		void initializeImGui() override;

		vk::Extent2D getRenderExtent() const { return m_render_extent; }
		vk::Format   getRenderColorFormat() const { return m_render_image.format; }
		vk::Format   getRenderDepthFormat() const { return m_depth_image.format; }

		const sAllocatedImage_vulkan& getDepthImage() const { return m_depth_image; }

		uint32_t                          getCurrentFrameIndex() const { return m_frame_number % m_frames_in_flight; }
		sFrameData_vulkan&                getCurrentFrame() { return m_frame_data[ getCurrentFrameIndex() ]; }
		std::vector< sFrameData_vulkan >& getFrameData() { return m_frame_data; }

		const vk::Instance&       getInstance() const { return m_instance.get(); }
		const vk::PhysicalDevice& getPhysicalDevice() const { return m_physical_device; }
		const vk::Device&         getLogicalDevice() const { return m_logical_device.get(); }
		const vma::Allocator&     getMemoryAllocator() const { return memory_allocator.get(); }

		const vk::Queue& getGraphicsQueue() const { return m_graphics_queue; }
		uint32_t         getGraphicsQueueFamily() const { return m_graphics_queue_family; }

		const iSampler* getLinearSampler() const { return m_sampler_linear; }

		const vk::DescriptorSet& getCurrentDescriptor() const { return m_descriptors[ getCurrentFrameIndex() ]; }

		PFN_vkGetInstanceProcAddr getInstanceProcAddr() const { return m_get_instance_proc_addr; }
		PFN_vkGetDeviceProcAddr   getDeviceProcAddr() const { return m_get_device_proc_addr; }

	protected:
		void renderDeferred( const vk::CommandBuffer& _command_buffer );
		void renderGui( const sPushConstantsGui& _push_constants, const cTexture2D* _texture ) override;

		void initializeDeferred() override;

		void createSwapchain( uint32_t _width, uint32_t _height );
		void createMemoryAllocator();

		void resize();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      _message_severity,
		                                                            VkDebugUtilsMessageTypeFlagsEXT             _message_type,
		                                                            const VkDebugUtilsMessengerCallbackDataEXT* _callback_data,
		                                                            void*                                       _user_data );

		vk::UniqueInstance m_instance;

		vk::Queue m_graphics_queue;
		uint32_t  m_graphics_queue_family;

		vk::UniqueSurfaceKHR m_surface;

		sAllocatedImage_vulkan m_depth_image;
		sAllocatedImage_vulkan m_render_image;
		vk::Extent2D           m_render_extent;

		vk::UniqueSwapchainKHR             m_swapchain;
		std::vector< vk::Image >           m_swapchain_images;
		std::vector< vk::UniqueImageView > m_swapchain_image_views;
		vk::Format                         m_swapchain_format;
		vk::Extent2D                       m_swapchain_extent;

		vk::PhysicalDevice   m_physical_device;
		vk::UniqueDevice     m_logical_device;
		vma::UniqueAllocator memory_allocator;

		iSampler* m_sampler_linear;

		uint32_t                         m_frames_in_flight;
		uint32_t                         m_frame_number;
		std::vector< sFrameData_vulkan > m_frame_data;

		sSubmitContext_vulkan m_submit_context;

		vk::UniqueDescriptorPool m_imgui_descriptor_pool;

		cCamera::eType                   m_last_camera_type;
		bool                             m_begin_deferred;
		std::vector< vk::DescriptorSet > m_descriptors;
		vk::UniqueDescriptorSetLayout    m_deferred_layout;

		PFN_vkGetInstanceProcAddr m_get_instance_proc_addr;
		PFN_vkGetDeviceProcAddr   m_get_device_proc_addr;

		vk::UniqueDebugUtilsMessengerEXT m_debug_messenger;

		sAllocatedBuffer_vulkan       m_vertex_buffer_gui;
		sAllocatedBuffer_vulkan       m_index_buffer_gui;
		vk::UniqueDescriptorSetLayout m_descriptor_layout_gui;
		cPipeline_vulkan*             m_pipeline_gui;
		cTexture2D*                   m_white_texture;
	};
}
