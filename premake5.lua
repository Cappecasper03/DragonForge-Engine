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

        filter {}
            flags {
                "FatalWarnings",
                "MultiProcessorCompile",
                "NoMinimalRebuild",
            }

            includedirs { "source/code" }
            editandcontinue "off"

        filter "configurations:Debug"
            targetname( project_name .. "-debug" )
            defines "DEBUG"
            optimize "off"
            symbols "On"
            warnings "Extra"
            
        filter "configurations:Release"
            targetname( project_name .. "-release" )
            defines "RELEASE"
            optimize "Speed"
            symbols "off"
            flags "LinkTimeOptimization"
