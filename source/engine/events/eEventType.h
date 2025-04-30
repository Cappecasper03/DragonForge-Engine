#pragma once

#include <SDL3/SDL_events.h>

namespace df::event
{
	enum eCategory
	{
		kApplication,
		kDisplay,
		kWindow,
		kKeyboard,
		kMouse,
		kJoystick,
		kGamepad,
		kTouch,
		kClipboard,
		kDragDrop,
		kAudioDevice,
		kSensor,
		kPen,
		kCameraDevice,
		kRender,
	};

	template< eCategory T >
	struct sEvent;

	template<>
	struct sEvent< kApplication >
	{
		enum eEnum
		{
			eQuit                = SDL_EVENT_QUIT,
			eTerminating         = SDL_EVENT_TERMINATING,
			eLowMemory           = SDL_EVENT_LOW_MEMORY,
			eWillEnterBackground = SDL_EVENT_WILL_ENTER_BACKGROUND,
			eDidEnterBackground  = SDL_EVENT_DID_ENTER_BACKGROUND,
			eWillEnterForeground = SDL_EVENT_WILL_ENTER_FOREGROUND,
			eDidEnterForeground  = SDL_EVENT_DID_ENTER_FOREGROUND,
			eLocaleChanged       = SDL_EVENT_LOCALE_CHANGED,
			eSystemThemeChanged  = SDL_EVENT_SYSTEM_THEME_CHANGED,
		};
	};

	template<>
	struct sEvent< kDisplay >
	{
		enum eEnum
		{
			eDisplayOrientation         = SDL_EVENT_DISPLAY_ORIENTATION,
			eDisplayAdded               = SDL_EVENT_DISPLAY_ADDED,
			eDisplayRemoved             = SDL_EVENT_DISPLAY_REMOVED,
			eDisplayMoved               = SDL_EVENT_DISPLAY_MOVED,
			eDisplayDesktopModeChanged  = SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,
			eDisplayCurrentModeChanged  = SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,
			eDisplayContentScaleChanged = SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,
			eDisplayFirst               = SDL_EVENT_DISPLAY_FIRST,
			eDisplayLast                = SDL_EVENT_DISPLAY_LAST,
		};
	};

	template<>
	struct sEvent< kWindow >
	{
		enum eEnum
		{
			eWindowShown               = SDL_EVENT_WINDOW_SHOWN,
			eWindowHidden              = SDL_EVENT_WINDOW_HIDDEN,
			eWindowExposed             = SDL_EVENT_WINDOW_EXPOSED,
			eWindowMoved               = SDL_EVENT_WINDOW_MOVED,
			eWindowResized             = SDL_EVENT_WINDOW_RESIZED,
			eWindowPixelSizeChanged    = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
			eWindowMetalViewResized    = SDL_EVENT_WINDOW_METAL_VIEW_RESIZED,
			eWindowMinimized           = SDL_EVENT_WINDOW_MINIMIZED,
			eWindowMaximized           = SDL_EVENT_WINDOW_MAXIMIZED,
			eWindowRestored            = SDL_EVENT_WINDOW_RESTORED,
			eWindowMouseEnter          = SDL_EVENT_WINDOW_MOUSE_ENTER,
			eWindowMouseLeave          = SDL_EVENT_WINDOW_MOUSE_LEAVE,
			eWindowFocusGained         = SDL_EVENT_WINDOW_FOCUS_GAINED,
			eWindowFocusLost           = SDL_EVENT_WINDOW_FOCUS_LOST,
			eWindowCloseRequested      = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
			eWindowHitTest             = SDL_EVENT_WINDOW_HIT_TEST,
			eWindowIccprofChanged      = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
			eWindowDisplayChanged      = SDL_EVENT_WINDOW_DISPLAY_CHANGED,
			eWindowDisplayScaleChanged = SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
			eWindowSafeAreaChanged     = SDL_EVENT_WINDOW_SAFE_AREA_CHANGED,
			eWindowOccluded            = SDL_EVENT_WINDOW_OCCLUDED,
			eWindowEnterFullscreen     = SDL_EVENT_WINDOW_ENTER_FULLSCREEN,
			eWindowLeaveFullscreen     = SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,
			eWindowDestroyed           = SDL_EVENT_WINDOW_DESTROYED,
			eWindowHdrStateChanged     = SDL_EVENT_WINDOW_HDR_STATE_CHANGED,
			eWindowFirst               = SDL_EVENT_WINDOW_FIRST,
			eWindowLast                = SDL_EVENT_WINDOW_LAST,
		};
	};

	template<>
	struct sEvent< kKeyboard >
	{
		enum eEnum
		{
			eKeyDown               = SDL_EVENT_KEY_DOWN,
			eKeyUp                 = SDL_EVENT_KEY_UP,
			eTextEditing           = SDL_EVENT_TEXT_EDITING,
			eTextInput             = SDL_EVENT_TEXT_INPUT,
			eKeymapChanged         = SDL_EVENT_KEYMAP_CHANGED,
			eKeyboardAdded         = SDL_EVENT_KEYBOARD_ADDED,
			eKeyboardRemoved       = SDL_EVENT_KEYBOARD_REMOVED,
			eTextEditingCandidates = SDL_EVENT_TEXT_EDITING_CANDIDATES,
		};
	};

	template<>
	struct sEvent< kMouse >
	{
		enum eEnum
		{
			eMouseMotion     = SDL_EVENT_MOUSE_MOTION,
			eMouseButtonDown = SDL_EVENT_MOUSE_BUTTON_DOWN,
			eMouseButtonUp   = SDL_EVENT_MOUSE_BUTTON_UP,
			eMouseWheel      = SDL_EVENT_MOUSE_WHEEL,
			eMouseAdded      = SDL_EVENT_MOUSE_ADDED,
			eMouseRemoved    = SDL_EVENT_MOUSE_REMOVED,
		};
	};

	template<>
	struct sEvent< kJoystick >
	{
		enum eEnum
		{
			eJoystickAxisMotion     = SDL_EVENT_JOYSTICK_AXIS_MOTION,
			eJoystickBallMotion     = SDL_EVENT_JOYSTICK_BALL_MOTION,
			eJoystickHatMotion      = SDL_EVENT_JOYSTICK_HAT_MOTION,
			eJoystickButtonDown     = SDL_EVENT_JOYSTICK_BUTTON_DOWN,
			eJoystickButtonUp       = SDL_EVENT_JOYSTICK_BUTTON_UP,
			eJoystickAdded          = SDL_EVENT_JOYSTICK_ADDED,
			eJoystickRemoved        = SDL_EVENT_JOYSTICK_REMOVED,
			eJoystickBatteryUpdated = SDL_EVENT_JOYSTICK_BATTERY_UPDATED,
			eJoystickUpdateComplete = SDL_EVENT_JOYSTICK_UPDATE_COMPLETE,
		};
	};

	template<>
	struct sEvent< kGamepad >
	{
		enum eEnum
		{
			eGamepadAxisMotion         = SDL_EVENT_GAMEPAD_AXIS_MOTION,
			eGamepadButtonDown         = SDL_EVENT_GAMEPAD_BUTTON_DOWN,
			eGamepadButtonUp           = SDL_EVENT_GAMEPAD_BUTTON_UP,
			eGamepadAdded              = SDL_EVENT_GAMEPAD_ADDED,
			eGamepadRemoved            = SDL_EVENT_GAMEPAD_REMOVED,
			eGamepadRemapped           = SDL_EVENT_GAMEPAD_REMAPPED,
			eGamepadTouchpadDown       = SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN,
			eGamepadTouchpadMotion     = SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION,
			eGamepadTouchpadUp         = SDL_EVENT_GAMEPAD_TOUCHPAD_UP,
			eGamepadSensorUpdate       = SDL_EVENT_GAMEPAD_SENSOR_UPDATE,
			eGamepadUpdateComplete     = SDL_EVENT_GAMEPAD_UPDATE_COMPLETE,
			eGamepadSteamHandleUpdated = SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED,
		};
	};

	template<>
	struct sEvent< kTouch >
	{
		enum eEnum
		{
			eFingerDown     = SDL_EVENT_FINGER_DOWN,
			eFingerUp       = SDL_EVENT_FINGER_UP,
			eFingerMotion   = SDL_EVENT_FINGER_MOTION,
			eFingerCanceled = SDL_EVENT_FINGER_CANCELED,
		};
	};

	template<>
	struct sEvent< kClipboard >
	{
		enum eEnum
		{
			eClipboardUpdate = SDL_EVENT_CLIPBOARD_UPDATE,
		};
	};

	template<>
	struct sEvent< kDragDrop >
	{
		enum eEnum
		{
			eDropFile     = SDL_EVENT_DROP_FILE,
			eDropText     = SDL_EVENT_DROP_TEXT,
			eDropBegin    = SDL_EVENT_DROP_BEGIN,
			eDropComplete = SDL_EVENT_DROP_COMPLETE,
			eDropPosition = SDL_EVENT_DROP_POSITION,
		};
	};

	template<>
	struct sEvent< kAudioDevice >
	{
		enum eEnum
		{
			eAudioDeviceAdded         = SDL_EVENT_AUDIO_DEVICE_ADDED,
			eAudioDeviceRemoved       = SDL_EVENT_AUDIO_DEVICE_REMOVED,
			eAudioDeviceFormatChanged = SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,
		};
	};

	template<>
	struct sEvent< kSensor >
	{
		enum eEnum
		{
			eSensorUpdate = SDL_EVENT_SENSOR_UPDATE,
		};
	};

	template<>
	struct sEvent< kPen >
	{
		enum eEnum
		{
			ePenProximityIn  = SDL_EVENT_PEN_PROXIMITY_IN,
			ePenProximityOut = SDL_EVENT_PEN_PROXIMITY_OUT,
			ePenDown         = SDL_EVENT_PEN_DOWN,
			ePenUp           = SDL_EVENT_PEN_UP,
			ePenButtonDown   = SDL_EVENT_PEN_BUTTON_DOWN,
			ePenButtonUp     = SDL_EVENT_PEN_BUTTON_UP,
			ePenMotion       = SDL_EVENT_PEN_MOTION,
			ePenAxis         = SDL_EVENT_PEN_AXIS,
		};
	};

	template<>
	struct sEvent< kCameraDevice >
	{
		enum eEnum
		{
			eCameraDeviceAdded    = SDL_EVENT_CAMERA_DEVICE_ADDED,
			eCameraDeviceRemoved  = SDL_EVENT_CAMERA_DEVICE_REMOVED,
			eCameraDeviceApproved = SDL_EVENT_CAMERA_DEVICE_APPROVED,
			eCameraDeviceDenied   = SDL_EVENT_CAMERA_DEVICE_DENIED,
		};
	};

	template<>
	struct sEvent< kRender >
	{
		enum eEnum
		{
			eRenderTargetsReset = SDL_EVENT_RENDER_TARGETS_RESET,
			eRenderDeviceReset  = SDL_EVENT_RENDER_DEVICE_RESET,
			eRenderDeviceLost   = SDL_EVENT_RENDER_DEVICE_LOST,
		};
	};
}