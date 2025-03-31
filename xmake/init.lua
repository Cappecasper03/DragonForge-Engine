function init_requires()
    add_requires(
        "assimp",
        "fmt",
        "freetype",
        "glm",
        "libsdl3",
        "stb" )
    
    add_requires(
        "vulkan-loader",
        "vulkan-headers",
        "vulkan-hpp",
        "vulkan-validationlayers",
        "vulkan-memory-allocator" )
    
    if is_plat( "windows" ) then
        add_requires( "tracy" )
    end

    add_requires( "glad", {
        configs={
            profile = "core"
        } } )

    add_requires( "imgui", {
        configs={
            opengl3 = true,
            vulkan  = true,
            sdl3    = true
        } } )

    add_defines( "VULKAN_HPP_NO_EXCEPTIONS" )
    add_defines( "VULKAN_HPP_DISPATCH_LOADER_DYNAMIC" )
    add_defines( "VULKAN_NO_PROTOTYPES" )
    add_defines( "VULKAN_HPP_NO_NODISCARD_WARNINGS" )
end

function init_target( _root )
    set_languages( "cxx23" )

    add_packages(
        "assimp",
        "fmt",
        "freetype",
        "glm",
        "libsdl3",
        "stb",
        "glad",
        "imgui" )
    
    add_packages(
        "vulkan-loader",
        "vulkan-headers",
        "vulkan-hpp",
        "vulkan-validationlayers",
        "vulkan-memory-allocator" )

    if is_plat( "windows" ) then
        add_packages( "tracy" )
    end

    if is_mode( "Debug" ) then
        add_defines( "DF_DEBUG" )
        
        set_symbols( "debug" )
        set_optimize( "none" )
    elseif is_mode( "Development" ) then 
        add_defines( "DF_DEVELOPMENT" )
        
        set_symbols( "debug" )
        set_optimize( "fast" )
        set_strip( "debug" )
    elseif is_mode( "Shipping" ) then 
        add_defines( "DF_SHIPPING" )
        
        set_symbols( "hidden" )
        set_optimize( "fastest" )
        set_strip( "all" )
    elseif is_mode( "Profiling" ) then 
        add_defines( "DF_PROFILING" )
        
        if is_plat( "windows" ) then
            add_defines( "TRACY_ENABLE" )
            add_defines( "TRACY_ONLY_LOCALHOST" )
            add_defines( "TRACY_VK_USE_SYMBOL_TABLE" )
        end
        
        set_symbols( "hidden" )
        set_optimize( "fastest" )
        set_strip( "all" )
    end
end