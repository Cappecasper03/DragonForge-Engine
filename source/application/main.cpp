#include <windows.h>

#include "core/filesystem/cFileSystem.h"

int main( int /*_argc*/, char* _argv[ ] )
{
    SetConsoleTitle( L"Logging" );

    const std::string argv_string( _argv[ 0 ] );
    vg::filesystem::setExecutableDirectory( argv_string.substr( 0, argv_string.find_last_of( '\\' ) ) + '\\' );

    return 0;
}
