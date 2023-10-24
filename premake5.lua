workspace_name = 'DragonForge-Engine'
project_name = 'DragonForge-Engine'

workspace( workspace_name )
    platforms { "Win64" }
    configurations { "Debug", "Release" }

    project( project_name )
        kind "WindowedApp"
        language "C++"
        cppdialect "C++20"
        location "build/vs"
        targetdir "game"
        debugdir "game"
        objdir "build/obj/%{cfg.buildcfg}"
        targetname( project_name )
        files { "source/**.cpp", "source/**.h", "source/**.hpp", "source/shaders/**.glsl" }
        flags {
            "FatalWarnings",
            "MultiProcessorCompile",
            "NoMinimalRebuild",
        }
        includedirs { "source" }
        editandcontinue "off"
        rtti "off"
        staticruntime "off"
        usefullpaths "off"
        externalwarnings "off"
        
        dofile "libraries.lua"

        prebuildcommands { 
            'del /q "../../game/data/shaders"',
            'pushd "../../source/shaders"',
            'for /r %%a in (*.glsl) do (',
                'echo f | xcopy /y "%%a" "../../game/data/shaders/%%~nxa"',
            ')',
            'popd'
        }
        
        filter "configurations:Debug"
            targetname( project_name .. "-debug" )
            defines "DEBUG"
            optimize "off"
            symbols "Full"
            warnings "Extra"
            
        filter "configurations:Release"
            targetname( project_name .. "-release" )
            defines "RELEASE"
            optimize "Speed"
            symbols "off"
            flags "LinkTimeOptimization"