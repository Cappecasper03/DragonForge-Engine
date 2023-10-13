#include "cFileSystem.h"

#include <filesystem>
#include <fstream>
#include <vector>

namespace vg::filesystem
{
    std::vector< std::string > s_data_directory;

    void addDataDirectory( const std::string& _path ) { s_data_directory.push_back( _path ); }

    std::fstream open( const std::string& _path, const std::ios::openmode _openmode )
    {
        std::fstream fstream;

        for( const std::string& data_directory : s_data_directory )
        {
            const std::string full_path = data_directory + _path;
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
            if( std::filesystem::exists( data_directory + _path ) )
                return true;
        }

        return false;
    }

    std::string read( const std::string& _path )
    {
        std::string  data;
        std::fstream fstream = open( _path, std::ios::in );

        if( !fstream.is_open() )
            return data;

        std::string line;
        while( std::getline( fstream, line ) )
            data += line;

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
        return std::remove( ( s_data_directory.front() + _path ).c_str() );
    }

    void create( const std::string& _path )
    {
        std::fstream fstream( s_data_directory.front() + _path, std::ios::out | std::ios::trunc );
    }
}
