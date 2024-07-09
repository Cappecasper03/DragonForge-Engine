local workspace_name = 'DragonForge-Engine'
local project_name = 'DragonForge-Engine'

local workspace_path = os.getcwd()

workspace( workspace_name )
    platforms     { 'Windows' }
    configurations{ 'Debug', 'Profiling', 'Release' }

    project( project_name )
        kind       'ConsoleApp'
        language   'C++'
        cppdialect 'C++latest'

        location  ( workspace_path .. '/build/vs' )
        targetdir ( workspace_path .. '/game/binaries' )
        debugdir  ( workspace_path .. '/game/binaries' )
        objdir    ( workspace_path .. '/build/obj/%{cfg.buildcfg}' )
        targetname( project_name )

        includedirs{ workspace_path .. '/source' }
        files      { workspace_path .. '/source/**' }
        
        defines{
            'VULKAN_HPP_NO_EXCEPTIONS',
            'VULKAN_HPP_DISPATCH_LOADER_DYNAMIC',
            'VULKAN_NO_PROTOTYPES',
            'VULKAN_HPP_NO_NODISCARD_WARNINGS',
        }
        
        flags{
            'FatalWarnings',
            'MultiProcessorCompile',
            'UndefinedIdentifiers',
        }

        editandcontinue   'off'
        rtti              'off'
        staticruntime     'off'
        usefullpaths      'off'
        externalwarnings  'off'

        postbuildcommands { 'powershell -ExecutionPolicy Bypass -File "' .. workspace_path .. '/utils/premake5/CreateExecutableShortcut.ps1" -projectFolder "' .. workspace_path .. '/" -executablePath $(TARGETPATH) -projectName ' .. project_name .. ' -WindowStyle Hidden' }

        filter 'configurations:Debug'
            targetname( project_name .. '-debug' )
            defines   { "DEBUG" }
            optimize  'Off'
            symbols   'Full'
            warnings  'Extra'

        filter 'configurations:Profiling'
            targetname( project_name .. '-profiling' )
            defines   { 'PROFILING', 'RELEASE', 'NDEBUG', 'TRACY_ENABLE', 'TRACY_ONLY_LOCALHOST' }
            optimize  'Speed'
            symbols   'On'
            flags     'LinkTimeOptimization'

        filter 'configurations:Release'
            targetname( project_name .. '-release' )
            defines   { 'RELEASE', 'NDEBUG' }
            optimize  'Speed'
            symbols   'Off'
            flags     'LinkTimeOptimization'

        filter 'platforms:Windows'
            system       'Windows'
            architecture 'x86_64'

        filter 'system:windows'
            files { workspace_path .. '/source/resources/executable.rc' }

        dofile 'libraries.lua'