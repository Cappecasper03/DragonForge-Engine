#include "cWindow_vulkan.h"

namespace df::vulkan
{
	bool cWindow_vulkan::create( const std::string& _window_name, unsigned _window_flags )
	{
		return iWindow::create( _window_name, _window_flags );
	}
}