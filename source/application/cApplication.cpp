#include "cApplication.h"

#include <windows.h>

#include "core/filesystem/cFileSystem.h"
#include "core/profiling/Profiling.h"

cApplication::cApplication()
{
    {
        size_t  size;
        wchar_t wbuffer[ MAX_PATH ];
        char    buffer[ MAX_PATH ];

        GetModuleFileName( nullptr, wbuffer, MAX_PATH );
        wcstombs_s( &size, buffer, MAX_PATH, wbuffer, MAX_PATH );

        const std::string executable_path( buffer );
        vg::filesystem::setExecutableDirectory( executable_path.substr( 0, executable_path.find_last_of( '\\' ) ) + '\\' );
    }

    {
        AllocConsole();
        FILE* file;
        freopen_s( &file, "CONOUT$", "w", stdout );
    }

    {
        vg::filesystem::remove( "logs.txt" );
        vg::filesystem::remove( "memory.txt" );
        vg::filesystem::remove( "profiling.txt" );
    }
}

cApplication::~cApplication()
{
    vg::profiling::printClear();
    vg::memory::printLeaks();
}

void cApplication::run()
{
    while( true )
    {
        update();
        render3D();
        render2D();
    }
}

void cApplication::update() {}

void cApplication::render3D() {}

void cApplication::render2D() {}
