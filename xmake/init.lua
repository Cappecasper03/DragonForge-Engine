function init_requires()
    add_requires( "assimp",
                "freetype",
                "glm",
                "libsdl3",
                "stb",
                "clay",
                "msdf-atlas-gen",
                "vulkan-headers",
                "vulkan-hpp",
                "vulkan-validationlayers",
                "vulkan-memory-allocator-hpp",
                "spirv-cross",
                "vulkan-loader",
                "slang" )
    
    if is_plat( "windows" ) and is_mode( "Profiling" ) then
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
    set_languages( "cxxlatest" )
    set_warnings( "all", "error" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )
    
    add_packages( "assimp",
                "freetype",
                "glm",
                "libsdl3",
                "stb",
                "glad",
                "imgui",
                "slang",
                "clay",
                "msdf-atlas-gen",
                "vulkan-loader",
                "vulkan-headers",
                "vulkan-hpp",
                "vulkan-validationlayers",
                "vulkan-memory-allocator-hpp",
                "spirv-cross" )
    
    if is_mode( "Debug" ) then
        add_defines( "DF_Debug" )
        
        set_symbols( "debug" )
        set_optimize( "none" )
    elseif is_mode( "Release" ) then 
        add_defines( "DF_Release" )
        
        set_symbols( "hidden" )
        set_optimize( "fastest" )
        set_strip( "all" )
    elseif is_mode( "Profiling" ) then 
        add_defines( "DF_Profiling" )
        
        if is_plat( "windows" ) then
            add_packages( "tracy" )

            add_defines( "TRACY_ENABLE" )
            add_defines( "TRACY_ONLY_LOCALHOST" )
            add_defines( "TRACY_VK_USE_SYMBOL_TABLE" )
        end
        
        set_symbols( "hidden" )
        set_optimize( "fastest" )
        set_strip( "all" )
    end
    
    if is_plat( "windows" ) then
        add_defines( "DF_Windows" )
    elseif is_plat( "linux" ) then
        add_defines( "DF_Linux" )
    end
end