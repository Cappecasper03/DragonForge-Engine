#include "Log.h"

#include <format>
#include <iostream>
#include <tracy/Tracy.hpp>
#include <windows.h>

#include "engine/filesystem/cFileSystem.h"

namespace df::log
{
	void print( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		ZoneScoped;

		printFile( _type, _function, _line, _message );
		printConsole( _type, _function, _line, _message );
	}

	void printFile( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		ZoneScoped;

		std::string message = {};
		std::string path    = {};

		switch( _type )
		{
			case kRaw: message = "[  RAW  ] "; break;
			case kMessage: message = "[MESSAGE] "; break;
			case kWarning: message = "[WARNING] "; break;
			case kError: message = "[ ERROR ] "; break;
		}

		switch( _type )
		{
			case kRaw:
			{
				message += _message;
				path = "log.txt";
			}
			break;
			case kMessage:
			case kWarning:
			case kError:
			{
				message += std::format( "{} Line {} - {} ", _function, _line, _message );
				path = "log.txt";
			}
			break;
		}

		message += '\n';
		filesystem::write( path, message, std::ios::out | std::ios::app );
	}

	void printConsole( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
	{
		ZoneScoped;

		std::string message     = {};
		WORD        attributes  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		int         tracy_color = 0xFFFFFF;

		switch( _type )
		{
			case kRaw: message = std::format( "[  RAW  ] {}", _message ); break;
			case kMessage: message = "[MESSAGE] "; break;
			case kWarning:
			{
				message     = "[WARNING] ";
				attributes  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				tracy_color = 0xFF0000 | 0x00FF00;
			}
			break;
			case kError:
			{
				message     = "[ ERROR ] ";
				attributes  = FOREGROUND_RED | FOREGROUND_INTENSITY;
				tracy_color = 0xFF0000;
			}
			break;
		}

		if( _type != kRaw )
			message += std::format( "{} Line {} - {} ", _function, _line, _message );

		TracyMessageC( message.data(), message.size(), tracy_color );

#ifdef DEBUG
		const HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

		SetConsoleTextAttribute( handle, attributes );
		std::cout << message << "\n";
#endif
	}
}
