#pragma once

#include <string>

namespace df::log
{
	enum eType
	{
		kRaw,
		kMessage,
		kWarning,
		kError,
	};

	extern void print( eType _type, const char* _function, unsigned _line, const std::string& _message );
	extern void printFile( eType _type, const char* _function, unsigned _line, const std::string& _message );
	extern void printConsole( eType _type, const char* _function, unsigned _line, const std::string& _message );
}

#define DF_LogRaw( ... )     df::log::print( df::log::eType::kRaw, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define DF_LogMessage( ... ) df::log::print( df::log::eType::kMessage, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define DF_LogWarning( ... ) df::log::print( df::log::eType::kWarning, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define DF_LogError( ... )   df::log::print( df::log::eType::kError, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )