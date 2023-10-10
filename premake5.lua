workspace_name = 'TEMPLATE_WORKSPACE' 
project_name = 'TEMPLATE_PROJECT' 

workspace( workspace_name )
    configurations { "Debug", "Release" }

    project( project_name )
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        location "build/vs"
        targetdir "game"
        debugdir "game"
        objdir "build/obj/%{cfg.buildcfg}"
        targetname( project_name )
        architecture "x86_64"
        files { "source/**.cpp", "source/**.h" }

        dofile "libraries.lua"

        flags {
            "FatalWarnings",
            "MultiProcessorCompile",
            "NoMinimalRebuild",
        }

        filter {}
            includedirs { "source/code" }

        filter "configurations:Debug"
            targetname( project_name .. "-debug" )
            defines "DEBUG"
            symbols "On"
            
        filter "configurations:Release"
            targetname( project_name .. "-release" )
            defines "NDEBUG"
            optimize "Full"
            symbols "off"
            flags "LinkTimeOptimization"