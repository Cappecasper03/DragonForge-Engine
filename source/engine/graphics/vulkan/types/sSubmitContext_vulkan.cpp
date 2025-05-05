#include "sSubmitContext_vulkan.h"

#include <SDL3/SDL_vulkan.h>

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"
#include "Helper_vulkan.h"

namespace df::vulkan
{
	void sSubmitContext_vulkan::create()
	{
		DF_ProfilingScopeCpu;

		create( reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() ) );
	}

	void sSubmitContext_vulkan::create( const cRenderer_vulkan* _renderer )
	{
		DF_ProfilingScopeCpu;

		const vk::Device& logical_device = _renderer->getLogicalDevice();

		fence = logical_device.createFenceUnique( helper::init::fenceCreateInfo() ).value;

		const vk::CommandPoolCreateInfo create_info = helper::init::commandPoolCreateInfo( _renderer->getGraphicsQueueFamily() );
		command_pool                                = logical_device.createCommandPoolUnique( create_info ).value;

		command_buffer.create( command_pool.get(), _renderer );

#ifdef DF_Profiling
		tracy_context = TracyVkContextCalibrated( _renderer->getInstance(),
		                                          _renderer->getPhysicalDevice(),
		                                          logical_device,
		                                          _renderer->getGraphicsQueue(),
		                                          command_buffer.get(),
		                                          _renderer->getInstanceProcAddr(),
		                                          _renderer->getDeviceProcAddr() );
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