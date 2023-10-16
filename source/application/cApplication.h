#pragma once

#include "core/misc/iSingleton.h"

class cApplication final : public vg::iSingleton< cApplication >
{
public:
    DISABLE_COPY_AND_MOVE( cApplication );

    cApplication();
    ~cApplication() override;

    void run();

private:
    void update();
    void render3D();
    void render2D();
};
