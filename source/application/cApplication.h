#pragma once

#include "core/misc/iSingleton.h"

struct GLFWwindow;

class cApplication final : public vg::iSingleton< cApplication >
{
public:
    DISABLE_COPY_AND_MOVE( cApplication );

    cApplication();
    ~cApplication() override;

    void run();

private:
    static void onResize( GLFWwindow* _window, const int _width, const int _height );

    void input();
    void update();

    void render3D();
    void render2D();

    GLFWwindow* m_window;
};