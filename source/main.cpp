#include <windows.h>

#include "application/cApplication.h"

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    cApplication application;
    application.run();

    return 0;
}
