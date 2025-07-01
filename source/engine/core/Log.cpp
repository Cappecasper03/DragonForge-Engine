#include "Log.h"

#include <print>

#include "cFileSystem.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	void cLog::print( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		printFile( _type, _function, _line, _message );

		printConsole( _type, _function, _line, _message );
	}

	void cLog::printFile( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

		std::string message = {};

		switch( _type )
		{
			case kRaw:     message = "[  RAW  ];;"; break;
			case kMessage: message = "[MESSAGE];;"; break;
			case kWarning: message = "[WARNING];;"; break;
			case kError:   message = "[ ERROR ];;"; break;
		}

		message += std::format( "{};;{};;{}\n", _function, _line, _message );
		cFileSystem::write( "binaries/log.csv", message, std::ios::out | std::ios::app );
	}

	void cLog::printConsole( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		DF_ProfilingScopeCpu;

#ifdef DF_Debug
		std::string message = {};

		switch( _type )
		{
			case kRaw:     message = std::format( "\033[0m[  RAW  ] {}", _message ); break;
			case kMessage: message = "\033[0m[MESSAGE] "; break;
			case kWarning: message = "\033[33m[WARNING] "; break;
			case kError:   message = "\033[31m[ ERROR ] "; break;
		}

		if( _type != kRaw )
			message += std::format( "{} Line {} - {}", _function, _line, _message );

		std::println( "{}", message );
#endif

#ifdef DF_Profiling
		std::string message     = {};
		int         tracy_color = 0xFFFFFF;

		switch( _type )
		{
			case kRaw:     message = fmt::format( "[  RAW  ] {}\n", _message ); break;
			case kMessage: message = "[MESSAGE] "; break;
			case kWarning:
			{
				message     = "[WARNING] ";
				tracy_color = 0xFF0000 | 0x00FF00;
			}
			break;
			case kError:
			{
				message     = "[ ERROR ] ";
				tracy_color = 0xFF0000;
			}
			break;
		}

		if( _type != kRaw )
			message += fmt::format( "{} Line {} - {}\n", _function, _line, _message );

		DF_ProfilingMessageColor( message, tracy_color );
#endif
	}
}
