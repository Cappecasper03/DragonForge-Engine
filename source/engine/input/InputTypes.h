#pragma once

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <unordered_map>

namespace df::input
{
	enum eCategory
	{
		kKeyboard,
		kMouse,
		kKeyModifier,
		kAction,
	};

	template< eCategory T >
	struct sInput
	{
		sInput() = delete;
	};

	template<>
	struct sInput< kKeyboard >
	{
		enum eEnum
		{
			kUnknown            = SDLK_UNKNOWN,
			kReturn             = SDLK_RETURN,
			kEscape             = SDLK_ESCAPE,
			kBackspace          = SDLK_BACKSPACE,
			kTab                = SDLK_TAB,
			kSpace              = SDLK_SPACE,
			kExclaim            = SDLK_EXCLAIM,
			kDblapostrophe      = SDLK_DBLAPOSTROPHE,
			kHash               = SDLK_HASH,
			kDollar             = SDLK_DOLLAR,
			kPercent            = SDLK_PERCENT,
			kAmpersand          = SDLK_AMPERSAND,
			kApostrophe         = SDLK_APOSTROPHE,
			kLeftparen          = SDLK_LEFTPAREN,
			kRightparen         = SDLK_RIGHTPAREN,
			kAsterisk           = SDLK_ASTERISK,
			kPlus               = SDLK_PLUS,
			kComma              = SDLK_COMMA,
			kMinus              = SDLK_MINUS,
			kPeriod             = SDLK_PERIOD,
			kSlash              = SDLK_SLASH,
			k0                  = SDLK_0,
			k1                  = SDLK_1,
			k2                  = SDLK_2,
			k3                  = SDLK_3,
			k4                  = SDLK_4,
			k5                  = SDLK_5,
			k6                  = SDLK_6,
			k7                  = SDLK_7,
			k8                  = SDLK_8,
			k9                  = SDLK_9,
			kColon              = SDLK_COLON,
			kSemicolon          = SDLK_SEMICOLON,
			kLess               = SDLK_LESS,
			kEquals             = SDLK_EQUALS,
			kGreater            = SDLK_GREATER,
			kQuestion           = SDLK_QUESTION,
			kAt                 = SDLK_AT,
			kLeftbracket        = SDLK_LEFTBRACKET,
			kBackslash          = SDLK_BACKSLASH,
			kRightbracket       = SDLK_RIGHTBRACKET,
			kCaret              = SDLK_CARET,
			kUnderscore         = SDLK_UNDERSCORE,
			kGrave              = SDLK_GRAVE,
			kA                  = SDLK_A,
			kB                  = SDLK_B,
			kC                  = SDLK_C,
			kD                  = SDLK_D,
			kE                  = SDLK_E,
			kF                  = SDLK_F,
			kG                  = SDLK_G,
			kH                  = SDLK_H,
			kI                  = SDLK_I,
			kJ                  = SDLK_J,
			kK                  = SDLK_K,
			kL                  = SDLK_L,
			kM                  = SDLK_M,
			kN                  = SDLK_N,
			kO                  = SDLK_O,
			kP                  = SDLK_P,
			kQ                  = SDLK_Q,
			kR                  = SDLK_R,
			kS                  = SDLK_S,
			kT                  = SDLK_T,
			kU                  = SDLK_U,
			kV                  = SDLK_V,
			kW                  = SDLK_W,
			kX                  = SDLK_X,
			kY                  = SDLK_Y,
			kZ                  = SDLK_Z,
			kLeftbrace          = SDLK_LEFTBRACE,
			kPipe               = SDLK_PIPE,
			kRightbrace         = SDLK_RIGHTBRACE,
			kTilde              = SDLK_TILDE,
			kDelete             = SDLK_DELETE,
			kPlusminus          = SDLK_PLUSMINUS,
			kCapslock           = SDLK_CAPSLOCK,
			kF1                 = SDLK_F1,
			kF2                 = SDLK_F2,
			kF3                 = SDLK_F3,
			kF4                 = SDLK_F4,
			kF5                 = SDLK_F5,
			kF6                 = SDLK_F6,
			kF7                 = SDLK_F7,
			kF8                 = SDLK_F8,
			kF9                 = SDLK_F9,
			kF10                = SDLK_F10,
			kF11                = SDLK_F11,
			kF12                = SDLK_F12,
			kPrintscreen        = SDLK_PRINTSCREEN,
			kScrolllock         = SDLK_SCROLLLOCK,
			kPause              = SDLK_PAUSE,
			kInsert             = SDLK_INSERT,
			kHome               = SDLK_HOME,
			kPageup             = SDLK_PAGEUP,
			kEnd                = SDLK_END,
			kPagedown           = SDLK_PAGEDOWN,
			kRight              = SDLK_RIGHT,
			kLeft               = SDLK_LEFT,
			kDown               = SDLK_DOWN,
			kUp                 = SDLK_UP,
			kNumlockclear       = SDLK_NUMLOCKCLEAR,
			kKpDivide           = SDLK_KP_DIVIDE,
			kKpMultiply         = SDLK_KP_MULTIPLY,
			kKpMinus            = SDLK_KP_MINUS,
			kKpPlus             = SDLK_KP_PLUS,
			kKpEnter            = SDLK_KP_ENTER,
			kKp1                = SDLK_KP_1,
			kKp2                = SDLK_KP_2,
			kKp3                = SDLK_KP_3,
			kKp4                = SDLK_KP_4,
			kKp5                = SDLK_KP_5,
			kKp6                = SDLK_KP_6,
			kKp7                = SDLK_KP_7,
			kKp8                = SDLK_KP_8,
			kKp9                = SDLK_KP_9,
			kKp0                = SDLK_KP_0,
			kKpPeriod           = SDLK_KP_PERIOD,
			kApplication        = SDLK_APPLICATION,
			kPower              = SDLK_POWER,
			kKpEquals           = SDLK_KP_EQUALS,
			kF13                = SDLK_F13,
			kF14                = SDLK_F14,
			kF15                = SDLK_F15,
			kF16                = SDLK_F16,
			kF17                = SDLK_F17,
			kF18                = SDLK_F18,
			kF19                = SDLK_F19,
			kF20                = SDLK_F20,
			kF21                = SDLK_F21,
			kF22                = SDLK_F22,
			kF23                = SDLK_F23,
			kF24                = SDLK_F24,
			kExecute            = SDLK_EXECUTE,
			kHelp               = SDLK_HELP,
			kMenu               = SDLK_MENU,
			kSelect             = SDLK_SELECT,
			kStop               = SDLK_STOP,
			kAgain              = SDLK_AGAIN,
			kUndo               = SDLK_UNDO,
			kCut                = SDLK_CUT,
			kCopy               = SDLK_COPY,
			kPaste              = SDLK_PASTE,
			kFind               = SDLK_FIND,
			kMute               = SDLK_MUTE,
			kVolumeup           = SDLK_VOLUMEUP,
			kVolumedown         = SDLK_VOLUMEDOWN,
			kKpComma            = SDLK_KP_COMMA,
			kKpEqualsas400      = SDLK_KP_EQUALSAS400,
			kAlterase           = SDLK_ALTERASE,
			kSysreq             = SDLK_SYSREQ,
			kCancel             = SDLK_CANCEL,
			kClear              = SDLK_CLEAR,
			kPrior              = SDLK_PRIOR,
			kReturn2            = SDLK_RETURN2,
			kSeparator          = SDLK_SEPARATOR,
			kOut                = SDLK_OUT,
			kOper               = SDLK_OPER,
			kClearagain         = SDLK_CLEARAGAIN,
			kCrsel              = SDLK_CRSEL,
			kExsel              = SDLK_EXSEL,
			kKp00               = SDLK_KP_00,
			kKp000              = SDLK_KP_000,
			kThousandsseparator = SDLK_THOUSANDSSEPARATOR,
			kDecimalseparator   = SDLK_DECIMALSEPARATOR,
			kCurrencyunit       = SDLK_CURRENCYUNIT,
			kCurrencysubunit    = SDLK_CURRENCYSUBUNIT,
			kKpLeftparen        = SDLK_KP_LEFTPAREN,
			kKpRightparen       = SDLK_KP_RIGHTPAREN,
			kKpLeftbrace        = SDLK_KP_LEFTBRACE,
			kKpRightbrace       = SDLK_KP_RIGHTBRACE,
			kKpTab              = SDLK_KP_TAB,
			kKpBackspace        = SDLK_KP_BACKSPACE,
			kKpA                = SDLK_KP_A,
			kKpB                = SDLK_KP_B,
			kKpC                = SDLK_KP_C,
			kKpD                = SDLK_KP_D,
			kKpE                = SDLK_KP_E,
			kKpF                = SDLK_KP_F,
			kKpXor              = SDLK_KP_XOR,
			kKpPower            = SDLK_KP_POWER,
			kKpPercent          = SDLK_KP_PERCENT,
			kKpLess             = SDLK_KP_LESS,
			kKpGreater          = SDLK_KP_GREATER,
			kKpAmpersand        = SDLK_KP_AMPERSAND,
			kKpDblampersand     = SDLK_KP_DBLAMPERSAND,
			kKpVerticalbar      = SDLK_KP_VERTICALBAR,
			kKpDblverticalbar   = SDLK_KP_DBLVERTICALBAR,
			kKpColon            = SDLK_KP_COLON,
			kKpHash             = SDLK_KP_HASH,
			kKpSpace            = SDLK_KP_SPACE,
			kKpAt               = SDLK_KP_AT,
			kKpExclam           = SDLK_KP_EXCLAM,
			kKpMemstore         = SDLK_KP_MEMSTORE,
			kKpMemrecall        = SDLK_KP_MEMRECALL,
			kKpMemclear         = SDLK_KP_MEMCLEAR,
			kKpMemadd           = SDLK_KP_MEMADD,
			kKpMemsubtract      = SDLK_KP_MEMSUBTRACT,
			kKpMemmultiply      = SDLK_KP_MEMMULTIPLY,
			kKpMemdivide        = SDLK_KP_MEMDIVIDE,
			kKpPlusminus        = SDLK_KP_PLUSMINUS,
			kKpClear            = SDLK_KP_CLEAR,
			kKpClearentry       = SDLK_KP_CLEARENTRY,
			kKpBinary           = SDLK_KP_BINARY,
			kKpOctal            = SDLK_KP_OCTAL,
			kKpDecimal          = SDLK_KP_DECIMAL,
			kKpHexadecimal      = SDLK_KP_HEXADECIMAL,
			kLctrl              = SDLK_LCTRL,
			kLshift             = SDLK_LSHIFT,
			kLalt               = SDLK_LALT,
			kLgui               = SDLK_LGUI,
			kRctrl              = SDLK_RCTRL,
			kRshift             = SDLK_RSHIFT,
			kRalt               = SDLK_RALT,
			kRgui               = SDLK_RGUI,
			kMode               = SDLK_MODE,
			kSleep              = SDLK_SLEEP,
			kWake               = SDLK_WAKE,
			kChannelIncrement   = SDLK_CHANNEL_INCREMENT,
			kChannelDecrement   = SDLK_CHANNEL_DECREMENT,
			kMediaPlay          = SDLK_MEDIA_PLAY,
			kMediaPause         = SDLK_PAUSE,
			kMediaRecord        = SDLK_MEDIA_RECORD,
			kMediaFastForward   = SDLK_MEDIA_FAST_FORWARD,
			kMediaRewind        = SDLK_MEDIA_REWIND,
			kMediaNextTrack     = SDLK_MEDIA_NEXT_TRACK,
			kMediaPreviousTrack = SDLK_MEDIA_PREVIOUS_TRACK,
			kMediaStop          = SDLK_STOP,
			kMediaEject         = SDLK_MEDIA_EJECT,
			kMediaPlayPause     = SDLK_MEDIA_PLAY_PAUSE,
			kMediaSelect        = SDLK_SELECT,
			kAcNew              = SDLK_AC_NEW,
			kAcOpen             = SDLK_AC_OPEN,
			kAcClose            = SDLK_AC_CLOSE,
			kAcExit             = SDLK_AC_EXIT,
			kAcSave             = SDLK_AC_SAVE,
			kAcPrint            = SDLK_AC_PRINT,
			kAcProperties       = SDLK_AC_PROPERTIES,
			kAcSearch           = SDLK_AC_SEARCH,
			kAcHome             = SDLK_HOME,
			kAcBack             = SDLK_AC_BACK,
			kAcForward          = SDLK_AC_FORWARD,
			kAcStop             = SDLK_STOP,
			kAcRefresh          = SDLK_AC_REFRESH,
			kAcBookmarks        = SDLK_AC_BOOKMARKS,
			kSoftleft           = SDLK_SOFTLEFT,
			kSoftright          = SDLK_SOFTRIGHT,
			kCall               = SDLK_CALL,
			kEndcall            = SDLK_ENDCALL,
			kLeftTab            = SDLK_LEFT_TAB,
			kLevel5Shift        = SDLK_LEVEL5_SHIFT,
			kMultiKeyCompose    = SDLK_MULTI_KEY_COMPOSE,
			kLmeta              = SDLK_LMETA,
			kRmeta              = SDLK_RMETA,
			kLhyper             = SDLK_LHYPER,
			kRhyper             = SDLK_RHYPER,
		};
	};

	template<>
	struct sInput< kMouse >
	{
		enum eEnum
		{
			kButtonLeft   = SDL_BUTTON_LEFT,
			kButtonMiddle = SDL_BUTTON_MIDDLE,
			kButtonRight  = SDL_BUTTON_RIGHT,
			kButtonX1     = SDL_BUTTON_X1,
			kButtonX2     = SDL_BUTTON_X2,
		};
	};

	template<>
	struct sInput< kKeyModifier >
	{
		enum eEnum
		{
			kNone   = SDL_KMOD_NONE,
			kLshift = SDL_KMOD_LSHIFT,
			kRshift = SDL_KMOD_RSHIFT,
			kLevel5 = SDL_KMOD_LEVEL5,
			kLctrl  = SDL_KMOD_LCTRL,
			kRctrl  = SDL_KMOD_RCTRL,
			kLalt   = SDL_KMOD_LALT,
			kRalt   = SDL_KMOD_RALT,
			kLgui   = SDL_KMOD_LGUI,
			kRgui   = SDL_KMOD_RGUI,
			kNum    = SDL_KMOD_NUM,
			kCaps   = SDL_KMOD_CAPS,
			kMode   = SDL_KMOD_MODE,
			kScroll = SDL_KMOD_SCROLL,
			kCtrl   = SDL_KMOD_CTRL,
			kShift  = SDL_KMOD_SHIFT,
			kAlt    = SDL_KMOD_ALT,
			kGui    = SDL_KMOD_GUI,
		};
	};

	template<>
	struct sInput< kAction >
	{
		enum eEnum
		{
			kRelease = -1,
			kNone    = 0,
			kPress   = 1,
			kRepeat  = 2,
		};
	};

	typedef sInput< kKeyboard >    sKeyboardInput;
	typedef sInput< kMouse >       sMouseInput;
	typedef sInput< kKeyModifier > sKeyModifierInput;
	typedef sInput< kAction >      sActionInput;

	struct sKeyboard
	{
		sKeyboardInput::eEnum    key       = sKeyboardInput::kUnknown;
		sActionInput::eEnum      action    = sActionInput::kNone;
		sKeyModifierInput::eEnum modifiers = sKeyModifierInput::kNone;
	};

	struct sMouseButton
	{
		sActionInput::eEnum action = sActionInput::kNone;
		unsigned            clicks = 0;
	};

	struct sMouseCursor
	{
		double x_delta = 0;
		double y_delta = 0;

		double x_previous = -1;
		double y_previous = -1;

		double x_current = -1;
		double y_current = -1;
	};

	struct sMouseScroll
	{
		double x_delta = 0;
		double y_delta = 0;
	};

	struct sInputs
	{
		std::unordered_map< sKeyboardInput::eEnum, sKeyboard > keyboard     = {};
		std::unordered_map< sMouseInput::eEnum, sMouseButton > mouse_button = {};
		sMouseCursor                                           mouse_cursor = {};
		sMouseScroll                                           mouse_scroll = {};
	};
}
