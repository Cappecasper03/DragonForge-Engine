#pragma once

#include <unordered_map>

namespace df::input
{
	enum eAction
	{
		ePress   = 1,
		eRelease = -1,
		eRepeat  = 2,
		eNone    = 0,
	};

	struct sKeyboard
	{
		int scancode = -1;
		int action   = -1;
		int mods     = -1;
	};

	struct sMouseButton
	{
		int action = -1;
		int clicks = -1;
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

	struct sInput
	{
		std::unordered_map< int, sKeyboard >    keyboard;
		std::unordered_map< int, sMouseButton > mouse_button;
		sMouseCursor                            mouse_cursor = {};
		sMouseScroll                            mouse_scroll = {};
	};
}
