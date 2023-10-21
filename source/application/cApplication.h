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

    const std::string& getName() { return m_name; }

private:
    void initialize();

    df::cTimer m_timer;

    std::string m_name;
};
