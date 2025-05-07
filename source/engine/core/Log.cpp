#include "Log.h"

#include <fmt/color.h>
#include <fmt/format.h>

#include "cFileSystem.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	void cLog::print( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		printFile( _type, _function, _line, _message );

#if defined( DF_Debug )
		printConsole( _type, _function, _line, _message );
#endif
	}

	void cLog::printFile( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		std::string message = {};

		switch( _type )
		{
			case kRaw:
				message = "[  RAW  ];;";
				break;
			case kMessage:
				message = "[MESSAGE];;";
				break;
			case kWarning:
				message = "[WARNING];;";
				break;
			case kError:
				message = "[ ERROR ];;";
				break;
		}

		message += fmt::format( "{};;{};;{}\n", _function, _line, _message );
		cFileSystem::write( "binaries/log.csv", message, std::ios::out | std::ios::app );
	}

	void cLog::printConsole( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		std::string message     = {};
		fmt::color  color       = fmt::color::white;
		int         tracy_color = 0xFFFFFF;

		switch( _type )
		{
			case kRaw:
				message = fmt::format( "[  RAW  ] {}\n", _message );
				break;
			case kMessage:
				message = "[MESSAGE] ";
				break;
			case kWarning:
			{
				message     = "[WARNING] ";
				color       = fmt::color::yellow;
				tracy_color = 0xFF0000 | 0x00FF00;
			}
			break;
			case kError:
			{
				message     = "[ ERROR ] ";
				color       = fmt::color::red;
				tracy_color = 0xFF0000;
			}
			break;
		}

		if( _type != kRaw )
			message += fmt::format( "{} Line {} - {}\n", _function, _line, _message );

		DF_ProfilingMessageColor( message, tracy_color );

#ifdef DF_Debug
		fmt::print( fmt::emphasis::faint | fg( color ), fmt::runtime( message ) );
#endif
	}
}
