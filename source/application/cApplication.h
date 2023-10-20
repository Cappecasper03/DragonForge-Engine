#pragma once

#include "core/misc/cTimer.h"
#include "core/misc/iSingleton.h"

struct GLFWwindow;

class cApplication final : public df::iSingleton< cApplication >
{
public:
    DISABLE_COPY_AND_MOVE( cApplication );

    cApplication();
    ~cApplication() override;

    void run();

private:
    static void framebufferSizeCallback( GLFWwindow* _window, const int _width, const int _height );

    void initializeOpenGL();

    GLFWwindow* m_window;

    df::cTimer m_timer;

    std::string m_name;
};
