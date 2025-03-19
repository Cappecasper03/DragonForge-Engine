function init_platforms()
    if is_mode( "Debug" ) then
        set_symbols "debug"
        set_optimize "none"
        set_strip "none"

        add_defines( "DF_DEBUG" )
    elseif is_mode( "Development" ) then 
        set_symbols "debug"
        set_optimize "fast"
        set_strip "debug"

        add_defines( "DF_DEVELOPMENT" )
    elseif is_mode( "Shipping" ) then 
        set_symbols "none"
        set_optimize "fastest"
        set_strip "all"

        add_defines( "DF_SHIPPING" )
    elseif is_mode( "Profiling" ) then 
        set_symbols "none"
        set_optimize "fastest"
        set_strip "all"

        add_defines( "DF_PROFILING" )
    end
end