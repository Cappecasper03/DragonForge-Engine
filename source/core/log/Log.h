#pragma once

#include <string>

namespace Log
{
    enum eType
    {
        kWarning,
        kMessage,
        kError,
        kProfiling,
        kMemory
    };

    extern void print( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message );
    extern void printFile( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message );
    extern void printConsole( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message );
};

#define LOG_WARNING( ... )   Log::print( Log::eType::kWarning,   __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_MESSAGE( ... )   Log::print( Log::eType::kMessage,   __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_ERROR( ... )     Log::print( Log::eType::kError,     __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_PROFILING( ... ) Log::print( Log::eType::kProfiling, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_MEMORY( ... )    Log::print( Log::eType::kMemory,    __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
