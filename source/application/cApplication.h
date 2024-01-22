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

    static void run();
    static void quit() { getInstance()->m_running = false; }

    static const std::string& getName() { return getInstance()->m_name; }

private:
    void initialize();

    df::cTimer m_timer;

    bool        m_running;
    std::string m_name;
};
