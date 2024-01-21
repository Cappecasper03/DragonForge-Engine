#include "Profiling.h"

#include <stack>
#include <vector>

#include "core/log/Log.h"
#include "core/misc/cTimer.h"

namespace df::profiling
{
    struct sProfiling
    {
        std::string message       = {};
        double      time_begin    = 0;
        double      time_total    = 0;
        uint32_t    call_count    = 0;
        double      time_min      = DBL_MAX;
        double      time_max      = DBL_MIN;
        double      time_external = 0;
    };

    cTimer   s_timer         = {};
    unsigned s_index_counter = 0;

    std::vector< sProfiling* > s_vector;
    std::stack< sProfiling* >  s_stack;

    void begin( const unsigned _index, const std::string& _message )
    {
        while( s_vector.size() < s_index_counter )
            s_vector.push_back( new sProfiling{} );

        sProfiling* profiling = s_vector[ _index ];
        profiling->time_begin = s_timer.getLifeSecond();

        if( profiling->message.empty() )
            profiling->message = _message;

        profiling->time_begin = s_timer.getLifeSecond();
        s_stack.push( profiling );
    }

    void end()
    {
        sProfiling* profiling = s_stack.top();

        const double time_end     = s_timer.getLifeSecond();
        const double time_elapsed = time_end - profiling->time_begin;

        profiling->time_total += time_elapsed;
        profiling->call_count++;
        profiling->time_min = profiling->time_min > time_elapsed ? time_elapsed : profiling->time_min;
        profiling->time_max = profiling->time_max < time_elapsed ? time_elapsed : profiling->time_max;

        s_stack.pop();
        if( !s_stack.empty() )
            s_stack.top()->time_external += time_elapsed;
    }

    unsigned generateIndex()
    {
        return s_index_counter++;
    }

    void printClear()
    {
        if( s_vector.empty() )
            return;

        LOG_PROFILING( std::format( "{}, {}, {}, {}, {}, {}, {}",
                          "Message",
                          "Call Count",
                          "Total",
                          "Average",
                          "Min",
                          "Max",
                          "External" ) );

        std::ranges::sort( s_vector,
                           []( const sProfiling* _a, const sProfiling* _b )
                           {
                               return _a->time_total > _b->time_total;
                           } );

        for( const sProfiling* profiling : s_vector )
        {
            LOG_PROFILING( std::format( "\"{}\", {}, {}, {}, {}, {}, {}",
                              profiling->message,
                              profiling->call_count,
                              profiling->time_total,
                              profiling->time_total / profiling->call_count,
                              profiling->time_min,
                              profiling->time_max,
                              profiling->time_external ) );
        }

        while( !s_stack.empty() )
            s_stack.pop();

        s_vector.clear();
    }
}
