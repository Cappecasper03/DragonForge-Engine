#include <windows.h>

#include "core/filesystem/cFileSystem.h"

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    size_t  size;
    wchar_t buffer[ MAX_PATH ];
    char    test[ MAX_PATH ];

    GetModuleFileName( nullptr, buffer, MAX_PATH );
    wcstombs_s( &size, test,MAX_PATH, buffer, MAX_PATH );

    const std::string executable_path( test );
    vg::filesystem::setExecutableDirectory( executable_path.substr( 0, executable_path.find_last_of( '\\' ) ) + '\\' );

    return 0;
}
