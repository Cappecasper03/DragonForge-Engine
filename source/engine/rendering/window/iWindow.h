#pragma once

#include <string>

#include "math/cVector.h"

struct SDL_Window;

namespace df
{

	class iWindow
	{
	public:
		iWindow();
		virtual ~iWindow();

		void             setSize( const cVector2i& _size );
		cVector2i&       getSize() { return m_window_size; }
		const cVector2i& getSize() const { return m_window_size; }

		void loadIcon() const;

		virtual bool create( const std::string& _window_name, unsigned _window_flags );

		void updateSize();

		SDL_Window* getWindow() const { return m_window; }

	protected:
		SDL_Window* m_window;
		cVector2i   m_window_size;
	};
}
