#pragma once

#include "engine/graphics/window/iWindow.h"

namespace df::vulkan
{
	class cWindow_vulkan final : public iWindow
	{
	public:
		DF_DeleteCopyAndMove( cWindow_vulkan );

		cWindow_vulkan()           = default;
		~cWindow_vulkan() override = default;

		bool create( const std::string& _window_name, unsigned _window_flags ) override;
	};
}
