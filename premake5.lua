include "settings.lua"

workspace( workspace_name )
    configurations { "Debug", "Release", "Final" }

    project( project_name )
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        location "build/vs"
        targetdir "build"
        objdir "build/obj/%{cfg.buildcfg}"
        targetname( project_name )
        architecture "x86_64"
        files { "source/**.cpp", "source/**.h" }

        dofile "libraries.lua"

        flags "MultiProcessorCompile"

        filter {}
            includedirs { "source/code" }

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

        filter "configurations:Final"
            defines { "NDEBUG" }
            optimize "Full"
            symbols "off"
            flags { "LinkTimeOptimization" }
