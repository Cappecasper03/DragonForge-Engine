#pragma once

#include <string>

namespace df
{
	class cLog
	{
	public:
		enum eType
		{
			kRaw,
			kMessage,
			kWarning,
			kError,
		};

		static void print( eType _type, const char* _function, unsigned _line, const std::string& _message );
		static void printFile( eType _type, const char* _function, unsigned _line, const std::string& _message );
		static void printConsole( eType _type, const char* _function, unsigned _line, const std::string& _message );
	};
}

#define DF_LogRaw( ... )     df::cLog::print( df::cLog::kRaw, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define DF_LogMessage( ... ) df::cLog::print( df::cLog::kMessage, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define DF_LogWarning( ... ) df::cLog::print( df::cLog::kWarning, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )
#define DF_LogError( ... )   df::cLog::print( df::cLog::kError, __FUNCTION__, __LINE__, std::string( __VA_ARGS__ ) )