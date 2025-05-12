#include "cFileSystem.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	std::string                      cFileSystem::s_game_directory;
	const std::vector< std::string > cFileSystem::s_folders{
		"data/", "data/models/", "data/textures/", "data/fonts/", "binaries/shaders/", "data/resources/",
	};

	void cFileSystem::setGameDirectory( const std::string& _path )
	{
		DF_ProfilingScopeCpu;

		s_game_directory = _path;
		std::ranges::replace( s_game_directory, '\\', '/' );
	}

	std::string cFileSystem::getPath( const std::string& _path, const std::vector< std::string >& _folders )
	{
		DF_ProfilingScopeCpu;

		std::string full_path = s_game_directory + _path;
		if( std::filesystem::exists( full_path ) )
			return full_path;

		const std::vector< std::string >* folders = &s_folders;
		if( !_folders.empty() )
			folders = &_folders;

		for( const std::string& folder_path: *folders )
		{
			full_path = s_game_directory + folder_path + _path;
			if( std::filesystem::exists( full_path ) )
				return full_path;
		}

		return _path;
	}

	std::fstream cFileSystem::open( const std::string& _path, const std::ios::openmode _open_mode )
	{
		DF_ProfilingScopeCpu;

		std::fstream fstream = {};
		fstream.open( s_game_directory + _path, _open_mode );
		return fstream;
	}

	bool cFileSystem::exists( const std::string& _path )
	{
		DF_ProfilingScopeCpu;

		if( std::filesystem::exists( getPath( _path, s_folders ) ) )
			return true;

		return false;
	}

	std::string cFileSystem::readAll( const std::string& _path, const std::string& _line_separator )
	{
		DF_ProfilingScopeCpu;

		std::string  data    = {};
		std::fstream fstream = open( _path, std::ios::in );

		if( !fstream.is_open() )
			return data;

		std::string line = {};
		while( std::getline( fstream, line ) )
			data += line + _line_separator;

		return data;
	}

	std::string cFileSystem::readContent( const std::string& _path, const std::string& _line_separator )
	{
		DF_ProfilingScopeCpu;

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

	void cFileSystem::write( const std::string& _path, const std::string& _message, const std::ios::openmode _open_mode )
	{
		DF_ProfilingScopeCpu;

		std::fstream fstream = open( _path, std::ios::out | _open_mode );

		if( !fstream.is_open() )
			return;

		fstream << _message;
	}
}
