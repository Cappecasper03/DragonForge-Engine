#pragma once

#include <SDL3/SDL_video.h>

#include "rendering/window/iWindow.h"

namespace df::opengl
{
	class cWindow_opengl final : public iWindow
	{
	public:
		enum eSwapInterval
		{
			kAdaptiveVerticalSync = -1,
			kImmediate            = 0,
			kVerticalSync         = 1,
		};

		cWindow_opengl() = default;
		~cWindow_opengl() override;

		static bool setSwapInterval( eSwapInterval _swap_interval );

		void        setViewport( const cVector2i& _offset = cVector2i( 0, 0 ) );
		static void setViewport( const cVector2i& _offset, const cVector2i& _size );

		bool create( const std::string& _window_name, unsigned _window_flags ) override;

		void swap();

		SDL_GLContext getContext() const { return m_context; }

	private:
		SDL_GLContext m_context;
	};
}
