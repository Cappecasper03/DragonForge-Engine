#pragma once

namespace df
{
	enum eEventType
	{
		eFirst = 0,

		/* Application events */
		eQuit = 0x100,
		eTerminating,
		eLowMemory,
		eWillEnterBackground,
		eDidEnterBackground,
		eWillEnterForeground,
		eDidEnterForeground,
		eLocaleChanged,
		eSystemThemeChanged,

		/* Display events */
		eDisplayOrientation = 0x151,
		eDisplayAdded,
		eDisplayRemoved,
		eDisplayMoved,
		eDisplayDesktopModeChanged,
		eDisplayCurrentModeChanged,
		eDisplayContentScaleChanged,
		eDisplayFirst = eDisplayOrientation,
		eDisplayLast  = eDisplayContentScaleChanged,

		/* Window events */
		eWindowShown = 0x202,
		eWindowHidden,
		eWindowExposed,
		eWindowMoved,
		eWindowResized,
		eWindowPixelSizeChanged,
		eWindowMetalViewResized,
		eWindowMinimized,
		eWindowMaximized,
		eWindowRestored,
		eWindowMouseEnter,
		eWindowMouseLeave,
		eWindowFocusGained,
		eWindowFocusLost,
		eWindowCloseRequested,
		eWindowHitTest,
		eWindowIccprofChanged,
		eWindowDisplayChanged,
		eWindowDisplayScaleChanged,
		eWindowSafeAreaChanged,
		eWindowOccluded,
		eWindowEnterFullscreen,
		eWindowLeaveFullscreen,
		eWindowDestroyed,
		eWindowHdrStateChanged,
		eWindowFirst = eWindowShown,
		eWindowLast  = eWindowHdrStateChanged,

		/* Keyboard events */
		eKeyDown = 0x300,
		eKeyUp,
		eTextEditing,
		eTextInput,
		eKeymapChanged,
		eKeyboardAdded,
		eKeyboardRemoved,
		eTextEditingCandidates,

		/* Mouse events */
		eMouseMotion = 0x400,
		eMouseButtonDown,
		eMouseButtonUp,
		eMouseWheel,
		eMouseAdded,
		eMouseRemoved,

		/* Joystick events */
		eJoystickAxisMotion = 0x600,
		eJoystickBallMotion,
		eJoystickHatMotion,
		eJoystickButtonDown,
		eJoystickButtonUp,
		eJoystickAdded,
		eJoystickRemoved,
		eJoystickBatteryUpdated,
		eJoystickUpdateComplete,

		/* Gamepad events */
		eGamepadAxisMotion = 0x650,
		eGamepadButtonDown,
		eGamepadButtonUp,
		eGamepadAdded,
		eGamepadRemoved,
		eGamepadRemapped,
		eGamepadTouchpadDown,
		eGamepadTouchpadMotion,
		eGamepadTouchpadUp,
		eGamepadSensorUpdate,
		eGamepadUpdateComplete,
		eGamepadSteamHandleUpdated,

		/* Touch events */
		eFingerDown = 0x700,
		eFingerUp,
		eFingerMotion,
		eFingerCanceled,

		/* Clipboard events */
		eClipboardUpdate = 0x900,

		/* Drag and drop events */
		eDropFile = 0x1000,
		eDropText,
		eDropBegin,
		eDropComplete,
		eDropPosition,

		/* Audio hotplug events */
		eAudioDeviceAdded = 0x1100,
		eAudioDeviceRemoved,
		eAudioDeviceFormatChanged,

		/* Sensor events */
		eSensorUpdate = 0x1200,

		/* Pressure-sensitive pen events */
		ePenProximityIn = 0x1300,
		ePenProximityOut,
		ePenDown,
		ePenUp,
		ePenButtonDown,
		ePenButtonUp,
		ePenMotion,
		ePenAxis,

		/* Camera hotplug events */
		eCameraDeviceAdded = 0x1400,
		eCameraDeviceRemoved,
		eCameraDeviceApproved,
		eCameraDeviceDenied,

		/* Render events */
		eRenderTargetsReset = 0x2000,
		eRenderDeviceReset,
		eRenderDeviceLost,

		/* Reserved events for private platforms */
		ePrivate0 = 0x4000,
		ePrivate1,
		ePrivate2,
		ePrivate3,

		/* Internal events */
		ePollSentinel = 0x7F00,
		eUser         = 0x8000,
		eLast         = 0xFFFF,
		eEnumPadding  = 0x7FFFFFFF,
	};
}