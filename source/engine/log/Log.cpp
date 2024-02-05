#include "Log.h"

#include <format>
#include <iostream>
#include <windows.h>

#include "engine/filesystem/cFileSystem.h"

#if PROFILING
#include "engine/profiling/Profiling.h"
#endif

namespace df::log
{
    void print( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        printFile( _type, _function, _line, _message );

#if defined ( DEBUG )
        printConsole( _type, _function, _line, _message );
#endif
    }

    void printFile( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        std::string message = {};
        std::string path    = {};

        switch( _type )
        {
            case kRaw: { message = "[  RAW  ] "; }
            break;
            case kMessage: { message = "[MESSAGE] "; }
            break;
            case kWarning: { message = "[WARNING] "; }
            break;
            case kError: { message = "[ ERROR ] "; }
            break;
            case kProfiling: { path = "profiling.csv"; }
            break;
            case kMemory: { path = "memory.txt"; }
            break;
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
            case kProfiling:
            case kMemory: { message = _message; }
            break;
        }

        message += '\n';
        filesystem::write( path, message, std::ios::out | std::ios::app );
    }

    void printConsole( const eType _type, const char* _function, const unsigned _line, const std::string& _message )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        std::string message    = {};
        WORD        attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

        switch( _type )
        {
            case kRaw: { message = std::format( "[  RAW  ] {}", _message ); }
            break;
            case kMessage: { message = "[MESSAGE] "; }
            break;
            case kWarning:
            {
                message    = "[WARNING] ";
                attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
            break;
            case kError:
            {
                message    = "[ ERROR ] ";
                attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            }
            break;
            default: return;
        }

        if( _type != kRaw )
            message += std::format( "{} Line {} - {} ", _function, _line, _message );

        const HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

        SetConsoleTextAttribute( handle, attributes );
        std::cout << message << std::endl;
        SetConsoleTextAttribute( handle,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
    }
}
