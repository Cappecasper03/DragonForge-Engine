#pragma once

#include "core/misc/iSingleton.h"

struct GLFWwindow;

class cApplication final : public df::iSingleton< cApplication >
{
public:
    DISABLE_COPY_AND_MOVE( cApplication );

    cApplication();
    ~cApplication() override;

    void run();

    GLFWwindow* getWindow() const { return m_window; }

private:
    static void onWindowResize( GLFWwindow* _window, const int _width, const int _height );

    void input();
    void update();

    void render();
    void render3D();
    void render2D();

    GLFWwindow* m_window;
};
