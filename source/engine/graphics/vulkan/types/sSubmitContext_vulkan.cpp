#include "sSubmitContext_vulkan.h"

#include <SDL3/SDL_vulkan.h>

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "Helper_vulkan.h"

namespace df::vulkan
{
	void sSubmitContext_vulkan::create()
	{
		DF_ProfilingScopeCpu;

		create( reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() ) );
	}

	void sSubmitContext_vulkan::create( const cGraphicsApi_vulkan* _graphics_api )
	{
		DF_ProfilingScopeCpu;

		const vk::Device& logical_device = _graphics_api->getLogicalDevice();

		fence = logical_device.createFenceUnique( helper::init::fenceCreateInfo() ).value;

		const vk::CommandPoolCreateInfo create_info = helper::init::commandPoolCreateInfo( _graphics_api->getGraphicsQueueFamily() );
		command_pool                                = logical_device.createCommandPoolUnique( create_info ).value;

		command_buffer.create( command_pool.get(), _graphics_api );

#ifdef DF_Profiling
		tracy_context = TracyVkContextCalibrated( _graphics_api->getInstance(),
		                                          _graphics_api->getPhysicalDevice(),
		                                          logical_device,
		                                          _graphics_api->getGraphicsQueue(),
		                                          command_buffer.get(),
		                                          _graphics_api->getInstanceProcAddr(),
		                                          _graphics_api->getDeviceProcAddr() );
#endif

		DF_LogMessage( "Created submit context" );
	}

	void sSubmitContext_vulkan::destroy()
	{
		DF_ProfilingScopeCpu;

		DF_DestroyProfilingContext( tracy_context );
		command_buffer.destroy();
		command_pool.reset();
		fence.reset();
	}
}