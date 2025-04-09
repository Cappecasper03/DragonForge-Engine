#include "Log.h"

#include <fmt/color.h>
#include <fmt/format.h>

#include "engine/filesystem/cFileSystem.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::log
{
	void print( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		printFile( _type, _function, _line, _message );

#if defined( DF_Debug ) || defined( DF_Profiling )
		printConsole( _type, _function, _line, _message );
#endif
	}

	void printFile( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		std::string message = {};

		switch( _type )
		{
			case eRaw:
				message = "[  RAW  ];;";
				break;
			case eMessage:
				message = "[MESSAGE];;";
				break;
			case eWarning:
				message = "[WARNING];;";
				break;
			case eError:
				message = "[ ERROR ];;";
				break;
		}

		message += fmt::format( "{};;{};;{}\n", _function, _line, _message );
		filesystem::write( "binaries/log.csv", message, std::ios::out | std::ios::app );
	}

	void printConsole( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		std::string message     = {};
		fmt::color  color       = fmt::color::white;
		int         tracy_color = 0xFFFFFF;

		switch( _type )
		{
			case eRaw:
				message = fmt::format( "[  RAW  ] {}\n", _message );
				break;
			case eMessage:
				message = "[MESSAGE] ";
				break;
			case eWarning:
			{
				message     = "[WARNING] ";
				color       = fmt::color::yellow;
				tracy_color = 0xFF0000 | 0x00FF00;
			}
			break;
			case eError:
			{
				message     = "[ ERROR ] ";
				color       = fmt::color::red;
				tracy_color = 0xFF0000;
			}
			break;
		}

		if( _type != eRaw )
			message += fmt::format( "{} Line {} - {}\n", _function, _line, _message );

		DF_ProfilingMessageColor( message, tracy_color );

#ifdef DF_Debug
		fmt::print( fmt::emphasis::faint | fg( color ), fmt::runtime( message ) );
#endif
	}
}
