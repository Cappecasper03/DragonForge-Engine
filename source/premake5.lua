local workspace_name = 'DragonForge-Engine'
local project_name = 'DragonForge-Engine'

local workspace_path = path.getdirectory( os.getcwd() )

workspace( workspace_name )
    platforms { "Win64" }
    configurations { "Debug", "Profiling", "Release" }

    project( project_name )
        kind      "WindowedApp"
        language   "C++"
        cppdialect "C++20"
        location   ( workspace_path .. "/build/vs" )
        targetdir  ( workspace_path .. "/game/binaries" )
        debugdir   ( workspace_path .. "/game/binaries" )
        objdir     ( workspace_path .. "/build/obj/%{cfg.buildcfg}" )
        targetname( project_name )
        files { "**.cpp", "**.h", "**.hpp", "shaders/**.glsl" }
        flags {
            "FatalWarnings",
            "MultiProcessorCompile",
            "NoMinimalRebuild",
        }
        includedirs       { workspace_path .. "/source" }
        editandcontinue   "off"
        rtti              "off"
        staticruntime     "off"
        usefullpaths      "off"
        externalwarnings  "off"
        prebuildcommands  { 'powershell -ExecutionPolicy Bypass -File "'.. workspace_path ..'/utils/premake5/prebuildcommands.ps1" -projectFolder "'.. workspace_path ..'/" -configuration "%{cfg.buildcfg}" -WindowStyle Hidden' }
        postbuildcommands { 'powershell -ExecutionPolicy Bypass -File "'.. workspace_path ..'/utils/premake5/postbuildcommands.ps1" -projectFolder "'.. workspace_path ..'/" -executablePath $(TARGETPATH) -projectName '.. project_name ..' -WindowStyle Hidden' }

        filter "configurations:Debug"
            targetname( project_name .. "-debug" )
            defines  "DEBUG"
            optimize "Off"
            symbols  "Full"
            warnings "Extra"

        filter "configurations:Profiling"
            targetname( project_name .. "-profiling" )
            defines  { "DEBUG", "PROFILING" }
            optimize "Off"
            symbols  "Full"
            warnings "Extra"

        filter "configurations:Release"
            targetname( project_name .. "-release" )
            defines  "RELEASE"
            optimize "Speed"
            symbols  "Off"
            flags    "LinkTimeOptimization"

        filter "system:windows"
            files { "resources/executable.rc" }

        dofile "libraries.lua"