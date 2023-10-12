#include "Profiling.h"

#include <stack>
#include <vector>

#include "core/memory/Memory.h"
#include "core/misc/cTimer.h"

namespace Profiling
{
    struct sProfiling
    {
        std::string message       = {};
        double      time_begin    = 0;
        double      time_total    = 0;
        double      time_external = 0;
        double      time_min      = 0;
        double      time_max      = 0;
        uint32_t    call_count    = 0;
    };

    cTimer*  timer           = NEW cTimer;
    unsigned index_counter   = 0;
    unsigned longest_message = 0;

    std::vector< sProfiling* > vector;
    std::stack< sProfiling* >  stack;

    void begin( const unsigned _index, const std::string& _message )
    {
        while( vector.size() < index_counter )
            vector.push_back( NEW sProfiling );

        sProfiling* profiling = vector[ _index ];

        if( profiling->message.empty() )
        {
            profiling->message = _message;

            while( longest_message < _message.length() )
                longest_message += 4;
        }

        profiling->time_begin = static_cast< double >( timer->getMilliLife() );
        stack.push( profiling );
    }

    void end()
    {
        sProfiling* profiling = stack.top();

        const double time_end     = static_cast< double >( timer->getMilliLife() );
        const double time_elapsed = time_end - profiling->time_begin;

        profiling->call_count++;
        profiling->time_total += time_elapsed;
        profiling->time_min = profiling->time_min > time_elapsed ? time_elapsed : profiling->time_min;
        profiling->time_max = profiling->time_max < time_elapsed ? time_elapsed : profiling->time_max;

        stack.pop();
        if( !stack.empty() )
            stack.top()->time_external += time_elapsed;
    }

    unsigned generateIndex()
    {
        return index_counter++;
    }
}
