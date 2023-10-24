#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <windows.h>

#include "application/cApplication.h"

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    cApplication application;
    application.run();

    return 0;
}
