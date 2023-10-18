#include "cFileSystem.h"

#include <filesystem>
#include <fstream>
#include <vector>

namespace df::filesystem
{
    std::string                s_executable_directory = {};
    std::vector< std::string > s_data_directory       = { "/", "data/", "data/shaders/" };

    void setExecutableDirectory( const std::string& _path ) { s_executable_directory = _path; }
    void addDataDirectory( const std::string& _path ) { s_data_directory.push_back( _path ); }

    std::fstream open( const std::string& _path, const std::ios::openmode _openmode )
    {
        std::fstream fstream = {};

        for( const std::string& data_directory : s_data_directory )
        {
            std::string full_path = s_executable_directory + data_directory;
            full_path.append( _path );

            fstream.open( full_path.c_str(), _openmode );
            if( fstream.is_open() )
                return fstream;
        }

        return fstream;
    }

    bool exists( const std::string& _path )
    {
        for( const std::string& data_directory : s_data_directory )
        {
            std::string full_path = s_executable_directory + data_directory;
            full_path.append( _path );

            if( std::filesystem::exists( full_path ) )
                return true;
        }

        return false;
    }

    std::string readAll( const std::string& _path, const std::string& _line_separator )
    {
        std::string  data    = {};
        std::fstream fstream = open( _path, std::ios::in );

        if( !fstream.is_open() )
            return data;

        std::string line = {};
        while( std::getline( fstream, line ) )
            data += line + _line_separator;

        return data;
    }

    std::string readContent( const std::string& _path, const std::string& _line_separator )
    {
        std::string  data    = {};
        std::fstream fstream = open( _path, std::ios::in );

        if( !fstream.is_open() )
            return data;

        std::string line = {};
        while( std::getline( fstream, line ) )
        {
            if( !line.empty() )
                data += line + _line_separator;
        }

        return data;
    }

    void write( const std::string& _path, const std::string& _message, const std::ios::openmode _openmode )
    {
        std::fstream fstream = open( _path, std::ios::out | _openmode );

        if( !fstream.is_open() )
            return;

        fstream << _message;
    }

    int remove( const std::string& _path )
    {
        return std::remove( ( s_executable_directory + _path ).c_str() );
    }
}
