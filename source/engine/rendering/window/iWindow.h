#pragma once

#include <string>

#include "engine/math/cVector.h"
#include "misc/Misc.h"

struct SDL_Window;

namespace df
{

	class iWindow
	{
		friend class cRenderer;

	public:
		DF_DisableCopyAndMove( iWindow );

		iWindow();
		virtual ~iWindow();

		void             setSize( const cVector2i& _size );
		cVector2i&       getSize() { return m_window_size; }
		const cVector2i& getSize() const { return m_window_size; }

		virtual bool create( const std::string& _window_name, unsigned _window_flags );

		void updateSize();

		static void setCaptureMouse( bool _capture );
		void        setRelativeMouseMode( bool _relative_mode ) const;

		SDL_Window* getWindow() const { return m_window; }

	protected:
		SDL_Window* m_window;
		cVector2i   m_window_size;

	private:
		void loadIcon() const;
	};
}
