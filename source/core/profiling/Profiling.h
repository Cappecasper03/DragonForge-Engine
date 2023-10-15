#pragma once

#include <string>

#include "core/misc/Misc.h"

namespace vg::profiling
{
    extern void     begin( const unsigned _index, const std::string& _message );
    extern void     end();
    extern unsigned generateIndex();

    extern void print();
    extern void clear();

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

#define PROFILING_SCOPE( _message ) static const unsigned PROFILING_UNIQUE_INDEX_NAME = vg::profiling::generateIndex(); \
                                    const vg::profiling::sProfilingScope PROFILING_UNIQUE_SCOPE_NAME( PROFILING_UNIQUE_INDEX_NAME, _message );

#define PROFILING_BEGIN( _message ) static const unsigned PROFILING_UNIQUE_INDEX_NAME = vg::profiling::generateIndex(); \
                                    vg::profiling::begin( PROFILING_UNIQUE_INDEX_NAME, _message );

#define PROFILING_END vg::profiling::end()
