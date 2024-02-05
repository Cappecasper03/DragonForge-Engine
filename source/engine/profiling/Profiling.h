#pragma once

#include <string>

#include "engine/misc/Misc.h"

namespace df::profiling
{
    extern void     begin( const unsigned _index, const std::string& _message );
    extern void     end();
    extern unsigned generateIndex();

    extern void printClear();

    struct sProfilingScope
    {
        DISABLE_COPY_AND_MOVE( sProfilingScope )

        sProfilingScope( const unsigned _index, const std::string& _message ) { begin( _index, _message ); }
        ~sProfilingScope() { end(); }
    };
};

#define PROFILING_UNIQUE( _name, _line ) _name ## _line
#define PROFILING_UNIQUE_INDEX( _line )  PROFILING_UNIQUE( _PROFILING_INDEX, _line )
#define PROFILING_UNIQUE_SCOPE( _line )  PROFILING_UNIQUE( _PROFILING_SCOPE, _line )

#define PROFILING_UNIQUE_INDEX_NAME PROFILING_UNIQUE_INDEX( __LINE__ )
#define PROFILING_UNIQUE_SCOPE_NAME PROFILING_UNIQUE_SCOPE( __LINE__ )

#define PROFILING_SCOPE( _message ) static const unsigned PROFILING_UNIQUE_INDEX_NAME = df::profiling::generateIndex(); \
                                    const df::profiling::sProfilingScope PROFILING_UNIQUE_SCOPE_NAME( PROFILING_UNIQUE_INDEX_NAME, _message );

#define PROFILING_BEGIN( _message ) static const unsigned PROFILING_UNIQUE_INDEX_NAME = df::profiling::generateIndex(); \
                                    df::profiling::begin( PROFILING_UNIQUE_INDEX_NAME, _message );

#define PROFILING_END df::profiling::end()
