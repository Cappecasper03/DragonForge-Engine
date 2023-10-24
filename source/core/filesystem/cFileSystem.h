#pragma once

#include <fstream>
#include <string>

namespace df::filesystem
{
    extern void setExecutableDirectory( const std::string& _path );

    extern std::fstream open( const std::string& _path, const std::ios::openmode _openmode = std::ios::in );
    extern bool         exists( const std::string& _path );
    extern std::string  readAll( const std::string& _path, const std::string& _line_separator = "" );
    extern std::string  readContent( const std::string& _path, const std::string& _line_separator = "" );
    extern void         write( const std::string& _path, const std::string& _message, const std::ios::openmode _openmode = std::ios::out );
    extern int          remove( const std::string& _path );
}
