workspace_name = 'DragonForge-Engine'
project_name = 'DragonForge-Engine'

workspace( workspace_name )
    platforms { "Win64" }
    configurations { "Debug", "Release" }

    project( project_name )
        kind      "WindowedApp"
        language   "C++"
        cppdialect "C++20"
        location   "build/vs"
        targetdir  "game/binaries"
        debugdir   "game/binaries"
        objdir     "build/obj/%{cfg.buildcfg}"
        targetname( project_name )
        files { "source/**.cpp", "source/**.h", "source/**.hpp", "source/shaders/**.glsl" }
        flags {
            "FatalWarnings",
            "MultiProcessorCompile",
            "NoMinimalRebuild",
        }
        includedirs       { "source" }
        editandcontinue   "off"
        rtti              "off"
        staticruntime     "off"
        usefullpaths      "off"
        externalwarnings  "off"
        buildoptions      { "/DPROFILING=1" }
        prebuildcommands  { 'powershell -ExecutionPolicy Bypass -File "'.. os.getcwd() ..'/utils/premake5/prebuildcommands.ps1" -projectFolder '.. os.getcwd() ..' -configuration "%{cfg.buildcfg}" -WindowStyle Hidden' }
        postbuildcommands { 'powershell -ExecutionPolicy Bypass -File "'.. os.getcwd() ..'/utils/premake5/postbuildcommands.ps1" -projectFolder '.. os.getcwd() ..' -executablePath $(TARGETPATH) -projectName '.. project_name ..' -WindowStyle Hidden' }

        filter "configurations:Debug"
            targetname( project_name .. "-debug" )
            defines  "DEBUG"
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
            files { "source/resources/executable.rc" }

        dofile "libraries.lua"