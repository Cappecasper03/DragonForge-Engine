include "settings.lua"

workspace(workspace_name)
    configurations { "Debug", "Release" }

project(project_name)
    kind "None"
    language "C++"
    location "build/vs"
    targetdir "build/%{cfg.buildcfg}"
    objdir "build/obj/%{cfg.buildcfg}"
    files { "source/**.cpp", "source/**.h" }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"