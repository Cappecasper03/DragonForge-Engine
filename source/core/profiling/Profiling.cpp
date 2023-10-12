#include "Profiling.h"

#include <stack>
#include <vector>

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

    unsigned index_counter = 0;

    std::vector< sProfiling* > profiling_vector;
    std::stack< sProfiling >  profiling_stack;

    void begin( const unsigned _index, const std::string& _message )
    {
        _index;
        _message;
    }

    void end()
    {}

    unsigned generateIndex()
    {
        return index_counter++;
    }
}
