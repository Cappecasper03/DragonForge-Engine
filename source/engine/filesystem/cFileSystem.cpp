#include "cFileSystem.h"

#include <filesystem>
#include <fstream>

namespace df::filesystem
{
    std::string                      s_game_directory = {};
    const std::vector< std::string > s_folders        = { "data/", "data/models/", "data/textures/", "data/fonts/", "binaries/shaders/", "data/resources/" };

    void setGameDirectory( const std::string& _path )
    {
        s_game_directory = _path;
        std::ranges::replace( s_game_directory, '\\', '/' );
    }

    std::string getPath( const std::string& _path, const std::vector< std::string >& _folders )
    {
        std::string full_path = s_game_directory + _path;
        if( std::filesystem::exists( full_path ) )
            return full_path;

        const std::vector< std::string >* folders = &s_folders;
        if( !_folders.empty() )
            folders = &_folders;

        for( const std::string& folder_path : *folders )
        {
            full_path = s_game_directory + folder_path + _path;
            if( std::filesystem::exists( full_path ) )
                return full_path;
        }

        return _path;
    }

    std::fstream open( const std::string& _path, const std::ios::openmode _openmode )
    {
        std::fstream fstream = {};
        fstream.open( getPath( _path ).c_str(), _openmode );
        return fstream;
    }

    bool exists( const std::string& _path )
    {
        if( std::filesystem::exists( getPath( _path ) ) )
            return true;

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
        return std::remove( getPath( _path ).c_str() );
    }
}
