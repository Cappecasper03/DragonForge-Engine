#pragma once

#include <fstream>
#include <string>

namespace vg::filesystem
{
    extern void addDataDirectory( const std::string& _path );

    extern std::fstream open( const std::string& _path, const std::ios::openmode _openmode = std::ios::in );
    extern bool         exists( const std::string& _path );
    extern std::string  read( const std::string& _path );
    extern void         write( const std::string& _path, const std::string& _message, const std::ios::openmode _openmode = std::ios::out );
    extern int          remove( const std::string& _path );
    extern void         create( const std::string& _path );
}
