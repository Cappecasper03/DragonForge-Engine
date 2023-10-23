#pragma once

#include <glm/vec2.hpp>

#include "core/misc/iSingleton.h"

struct GLFWwindow;

namespace df
{
    class cRenderer final : public iSingleton< cRenderer >
    {
    public:
        DISABLE_COPY_AND_MOVE( cRenderer );

        cRenderer();
        ~cRenderer() override;

        void render() const;

        GLFWwindow* getWindow() const { return m_window; }

        void resizeWindow( const int& _width = -1, const int& _height = -1 ) const;

        void setCursorInputMode( const int& _value ) const;

    private:
        static void framebufferSizeCallback( GLFWwindow* _window, const int _width, const int _height );

        GLFWwindow* m_window;
        glm::ivec2  m_window_size;
    };
};
