#pragma once

#include <string>

namespace vg::log
{
    enum eType
    {
        kMessage,
        kWarning,
        kError,
        kProfiling,
        kMemory
    };

    extern void print( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message );
    extern void printFile( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message );
    extern void printConsole( const eType _type, const char* _func_name, const unsigned _line, const std::string& _message );
}

#define LOG_MESSAGE( ... )   vg::log::print( vg::log::eType::kMessage,   __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_WARNING( ... )   vg::log::print( vg::log::eType::kWarning,   __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_ERROR( ... )     vg::log::print( vg::log::eType::kError,     __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_PROFILING( ... ) vg::log::print( vg::log::eType::kProfiling, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define LOG_MEMORY( ... )    vg::log::print( vg::log::eType::kMemory,    __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
