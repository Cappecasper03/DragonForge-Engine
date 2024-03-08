#pragma once

#include <glm/vec2.hpp>

#include "engine/misc/Misc.h"

struct GLFWwindow;

namespace df
{
	class iFramebuffer;
	class iQuad;

	class iRenderer
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( iRenderer )

		iRenderer()          = default;
		virtual ~iRenderer() = default;

		virtual void render() = 0;

		virtual void*             getWindow() { return nullptr; }
		virtual const glm::ivec2& getWindowSize() = 0;

		virtual void resizeWindow( int /*_width*/ = -1, int /*_height*/ = -1 ) {}
		virtual void setCursorInputMode( int /*_value*/ ) {}

		bool                isDeferred() const { return m_use_deferred; }
		const iFramebuffer* getFramebuffer() const { return m_framebuffer; }

	protected:
		glm::ivec2 m_window_size = { 1200, 800 };

		bool          m_use_deferred = false;
		iFramebuffer* m_framebuffer  = nullptr;
		iQuad*        m_screen_quad  = nullptr;

		bool m_window_resized = false;

		inline static int m_glfw_use_count = 0;
	};
}
