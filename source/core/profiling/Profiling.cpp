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
        double      time_external = 0;
        double      time_min      = DBL_MAX;
        double      time_max      = DBL_MIN;
        uint32_t    call_count    = 0;
    };

    cTimer   s_timer         = {};
    unsigned s_index_counter = 0;

    std::vector< sProfiling* > s_vector;
    std::stack< sProfiling* >  s_stack;

    void begin( const unsigned _index, const std::string& _message )
    {
        while( s_vector.size() < s_index_counter )
            s_vector.push_back( new sProfiling );

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

        profiling->call_count++;
        profiling->time_total += time_elapsed;
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

        size_t longest_message    = 7;
        size_t longest_call_count = 10;
        size_t longest_average    = 7;
        size_t longest_min        = 3;
        size_t longest_max        = 3;
        size_t longest_total      = 5;
        size_t longest_external   = 8;

        for( const sProfiling* profiling : s_vector )
        {
            longest_message = profiling->message.length() > longest_message ? profiling->message.length() : longest_message;

            const size_t call_count = std::format( "{}", profiling->call_count ).length();
            longest_call_count      = call_count > longest_call_count ? call_count : longest_call_count;

            const size_t average = std::format( "{}", profiling->time_total / profiling->call_count ).length();
            longest_average      = average > longest_average ? average : longest_average;

            const size_t min = std::format( "{}", profiling->time_min ).length();
            longest_min      = min > longest_min ? min : longest_min;

            const size_t max = std::format( "{}", profiling->time_max ).length();
            longest_max      = max > longest_max ? max : longest_max;

            const size_t total = std::format( "{}", profiling->time_total ).length();
            longest_total      = total > longest_total ? total : longest_total;

            const size_t external = std::format( "{}", profiling->time_external ).length();
            longest_external      = external > longest_external ? external : longest_external;
        }

        longest_message    += 5;
        longest_call_count += 5;
        longest_average    += 5;
        longest_min        += 5;
        longest_max        += 5;
        longest_total      += 5;
        longest_external   += 5;

        LOG_PROFILING( std::format( "{:<{}} {:<{}} {:<{}} {:<{}} {:<{}} {:<{}} {:<{}}",
                          "Message", longest_message,
                          "Call Count", longest_call_count,
                          "Average", longest_average,
                          "Min", longest_min,
                          "Max", longest_max,
                          "Total", longest_total,
                          "External", longest_external ) );
        LOG_PROFILING( "" );

        for( const sProfiling* profiling : s_vector )
        {
            LOG_PROFILING( std::format( "{:<{}} {:<{}} {:<{}} {:<{}} {:<{}} {:<{}} {:<{}}",
                              profiling->message, longest_message,
                              profiling->call_count, longest_call_count,
                              profiling->time_total / profiling->call_count, longest_average,
                              profiling->time_min, longest_min,
                              profiling->time_max, longest_max,
                              profiling->time_total, longest_total,
                              profiling->time_external, longest_external ) );
        }

        while( !s_stack.empty() )
            s_stack.pop();

        for( const sProfiling* profiling : s_vector )
            delete profiling;

        s_vector.clear();
    }
}
