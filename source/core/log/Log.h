#pragma once

#include <string>

namespace df::log
{
    enum eType
    {
        kMessage,
        kWarning,
        kError,
        kProfiling,
        kMemory
    };

    extern void print( const eType _type, const char* _function, const unsigned _line, const std::string& _message );
    extern void printFile( const eType _type, const char* _function, const unsigned _line, const std::string& _message );
    extern void printConsole( const eType _type, const char* _function, const unsigned _line, const std::string& _message );
}

#define LOG_MESSAGE( ... )   df::log::print( df::log::eType::kMessage,   __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_WARNING( ... )   df::log::print( df::log::eType::kWarning,   __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_ERROR( ... )     df::log::print( df::log::eType::kError,     __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_PROFILING( ... ) df::log::print( df::log::eType::kProfiling, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_MEMORY( ... )    df::log::print( df::log::eType::kMemory,    __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
