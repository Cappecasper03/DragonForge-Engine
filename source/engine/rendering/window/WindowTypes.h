#pragma once

namespace df::window
{
	enum eFlags : unsigned int
	{
		kFullscreen        = 0x0000000000000001,
		kOpenGl            = 0x0000000000000002,
		kOccluded          = 0x0000000000000004,
		kHidden            = 0x0000000000000008,
		kBorderless        = 0x0000000000000010,
		kResizable         = 0x0000000000000020,
		kMinimized         = 0x0000000000000040,
		kMaximized         = 0x0000000000000080,
		kMouseGrabbed      = 0x0000000000000100,
		kInputFocus        = 0x0000000000000200,
		kMouseFocus        = 0x0000000000000400,
		kExternal          = 0x0000000000000800,
		kModal             = 0x0000000000001000,
		kHighPixelDensity  = 0x0000000000002000,
		kMouseCapture      = 0x0000000000004000,
		kMouseRelativeMode = 0x0000000000008000,
		kAlwaysOnTop       = 0x0000000000010000,
		kUtility           = 0x0000000000020000,
		kTooltip           = 0x0000000000040000,
		kPopupMenu         = 0x0000000000080000,
		kKeyboardGrabbed   = 0x0000000000100000,
		kVulkan            = 0x0000000010000000,
		kMetal             = 0x0000000020000000,
		kTransparent       = 0x0000000040000000,
		kNotFocusable      = 0x0000000080000000,
	};
}
