#pragma once

#include <glm/vec2.hpp>

#include "engine/misc/iSingleton.h"

struct GLFWwindow;

namespace df
{
    class cFramebuffer;
    class cQuad;

    class cRenderer final : public iSingleton< cRenderer >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cRenderer );

        cRenderer();
        ~cRenderer() override;

        static void render();

        static GLFWwindow*       getWindow() { return getInstance()->m_window; }
        static const glm::ivec2& getWindowSize() { return getInstance()->m_window_size; }

        static const cFramebuffer* getFramebuffer() { return getInstance()->m_framebuffer; }

        static void resizeWindow( const int& _width = -1, const int& _height = -1 );

        static void setCursorInputMode( const int& _value );

    private:
        static void framebufferSizeCallback( GLFWwindow* _window, const int _width, const int _height );
        static void debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _user_param );

        void initializeDeferred();

        GLFWwindow* m_window;
        glm::ivec2  m_window_size;

        cFramebuffer* m_framebuffer;
        cQuad*        m_screen_quad;
    };
};
