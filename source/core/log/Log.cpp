#include "Log.h"

#include <iostream>
#include <windows.h>

namespace vg::log
{
    void print( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message )
    {
        printFile( _type, _func_name, _line, _message );
        printConsole( _type, _func_name, _line, _message );
    }

    void printFile( const eType /*_type*/, const char* /*_func_name*/, const unsigned /*_line*/, const std::string& /*_message*/ )
    {
        std::string message = {};
    }

    void printConsole( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message )
    {
        std::string message    = {};
        WORD        attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

        switch( _type )
        {
            case kWarning:
            {
                message    = "[ WARNING ] ";
                attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
            break;
            case kError:
            {
                message    = "[  ERROR  ] ";
                attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            }
            break;
            case kMessage: { message = "[ MESSAGE ] "; }
            break;
            default: return;
        }

        message += std::string( _func_name ) + " Line " + std::to_string( _line ) + " - " + _message;

        const HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

        SetConsoleTextAttribute( handle, attributes );
        std::cout << message << std::endl;
        SetConsoleTextAttribute( handle,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
    }
}
