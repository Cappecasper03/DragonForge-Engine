#pragma once

#include "engine/misc/cTimer.h"
#include "engine/misc/iSingleton.h"

struct GLFWwindow;

class cApplication final : public df::iSingleton< cApplication >
{
public:
    DF_DISABLE_COPY_AND_MOVE( cApplication );

    cApplication();
    ~cApplication() override;

    static void run();
    static void quit() { getInstance()->m_running = false; }

    static const std::string& getName() { return getInstance()->m_name; }

private:
    void initializeEngine();

    df::cTimer m_timer;

    bool        m_running;
    std::string m_name;
};
