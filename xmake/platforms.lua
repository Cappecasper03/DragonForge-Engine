function init_platforms()
    if is_mode( "Debug" ) then
        add_defines( "DF_DEBUG" )
        
        set_symbols "debug"
        set_optimize "none"
        set_strip "none"
    elseif is_mode( "Development" ) then 
        add_defines( "DF_DEVELOPMENT" )
        
        set_symbols "debug"
        set_optimize "fast"
        set_strip "debug"
    elseif is_mode( "Shipping" ) then 
        add_defines( "DF_SHIPPING" )
        
        set_symbols "none"
        set_optimize "fastest"
        set_strip "all"
    elseif is_mode( "Profiling" ) then 
        add_defines( "DF_PROFILING" )
        
        set_symbols "none"
        set_optimize "fastest"
        set_strip "all"
    end
    
    add_requires(
        "assimp",
        "fmt",
        "freetype",
        "glad",
        "glm",
        "imgui",
        "libsdl3",
        "stb",
        "tracy",
        "vulkansdk",
        "vulkan-memory-allocator-hpp" )
end

function target_platform( _root )
    set_languages( "cxx23" )

    add_packages(
        "assimp",
        "fmt",
        "freetype",
        "glad",
        "glm",
        "imgui",
        "libsdl3",
        "stb",
        "tracy",
        "vulkansdk",
        "vulkan-memory-allocator-hpp" )
end