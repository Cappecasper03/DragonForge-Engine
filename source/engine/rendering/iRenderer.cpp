#include "iRenderer.h"

#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>

namespace df
{
	void iRenderer::setCursorInputMode( const int _mode ) const
	{
		ZoneScoped;

		glfwSetInputMode( m_window, GLFW_CURSOR, _mode );
	}
}
