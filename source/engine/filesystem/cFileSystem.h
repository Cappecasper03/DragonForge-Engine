#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace df
{
	class cFileSystem
	{
	public:
		static void               setGameDirectory( const std::string& _path );
		static const std::string& getGameDirectory() { return s_game_directory; }

		static std::string getPath( const std::string& _path, const std::vector< std::string >& _folders = {} );

		static std::fstream open( const std::string& _path, std::ios::openmode _open_mode = std::ios::in );
		static bool         exists( const std::string& _path );

		static std::string readAll( const std::string& _path, const std::string& _line_separator = "" );
		static std::string readContent( const std::string& _path, const std::string& _line_separator = "" );

		static void write( const std::string& _path, const std::string& _message, std::ios::openmode _open_mode = std::ios::out );
		static int  remove( const std::string& _path ) { return std::remove( getPath( _path, s_folders ).c_str() ); }

	private:
		static std::string                      s_game_directory;
		static const std::vector< std::string > s_folders;
	};
}
