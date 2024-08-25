project 'application'
    kind       'WindowedApp'
    cppdialect 'C++Latest'

    targetdir ( '../../game/binaries' )
    location  ( '../../build/%{prj.name}' )
    objdir    ( '../../build/%{prj.name}/' .. OutputDir )

    files
    {
        '*/**',
    }

    includedirs
    {
        '../../source',
        '../application',
    }

    externalincludedirs
    {
        '../../utils/*/include',
        '../../utils/*/source',
    }

    libdirs
    {
        '../../build/lib'
    }

    links
    {
        LibraryPaths,
        "engine",
    }

    filter 'configurations:Debug'
        targetname ( WorkspaceName .. '-debug' )
        optimize   'Off'
        symbols    'Full'
        warnings   'Extra'
        runtime    'Debug'

        defines
        {
            'DEBUG',
        }

    filter 'configurations:Profiling'
        targetname ( WorkspaceName .. '-profiling' )
        optimize   'Speed'
        symbols    'Off'
        runtime    'Release'

        flags
        {
            'LinkTimeOptimization',
        }

        defines
        {
            'RELEASE',
            'NDEBUG',
            'PROFILING',
            'TRACY_ENABLE',
            'TRACY_ONLY_LOCALHOST',
            'TRACY_VK_USE_SYMBOL_TABLE',
        }

    filter 'configurations:Release'
        targetname ( WorkspaceName .. '-release' )
        optimize   'Speed'
        symbols    'Off'
        runtime    'Release'

        flags
        {
            'LinkTimeOptimization',
        }

        defines
        {
            'RELEASE',
            'NDEBUG',
        }

    filter 'platforms:Windows'
        postbuildcommands
        {
            'powershell -ExecutionPolicy Bypass -File "../../utils/premake5/CreateExecutableShortcut.ps1" -projectFolder "../../" -executablePath $(TARGETPATH) -projectName ' .. WorkspaceName .. ' -WindowStyle Hidden',
        }