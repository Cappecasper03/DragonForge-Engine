#pragma once

#include <SDL3/SDL_video.h>

namespace df::window
{
	enum eFlags
	{
		kFullscreen        = SDL_WINDOW_FULLSCREEN,
		kOpenGl            = SDL_WINDOW_OPENGL,
		kOccluded          = SDL_WINDOW_OCCLUDED,
		kHidden            = SDL_WINDOW_HIDDEN,
		kBorderless        = SDL_WINDOW_BORDERLESS,
		kResizable         = SDL_WINDOW_RESIZABLE,
		kMinimized         = SDL_WINDOW_MINIMIZED,
		kMaximized         = SDL_WINDOW_MAXIMIZED,
		kMouseGrabbed      = SDL_WINDOW_MOUSE_GRABBED,
		kInputFocus        = SDL_WINDOW_INPUT_FOCUS,
		kMouseFocus        = SDL_WINDOW_MOUSE_FOCUS,
		kExternal          = SDL_WINDOW_EXTERNAL,
		kModal             = SDL_WINDOW_MODAL,
		kHighPixelDensity  = SDL_WINDOW_HIGH_PIXEL_DENSITY,
		kMouseCapture      = SDL_WINDOW_MOUSE_CAPTURE,
		kMouseRelativeMode = SDL_WINDOW_MOUSE_RELATIVE_MODE,
		kAlwaysOnTop       = SDL_WINDOW_ALWAYS_ON_TOP,
		kUtility           = SDL_WINDOW_UTILITY,
		kTooltip           = SDL_WINDOW_TOOLTIP,
		kPopupMenu         = SDL_WINDOW_POPUP_MENU,
		kKeyboardGrabbed   = SDL_WINDOW_KEYBOARD_GRABBED,
		kVulkan            = SDL_WINDOW_VULKAN,
		kMetal             = SDL_WINDOW_METAL,
		kTransparent       = SDL_WINDOW_TRANSPARENT,
		kNotFocusable      = SDL_WINDOW_NOT_FOCUSABLE,
	};
}
