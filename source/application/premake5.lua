project 'application'
    kind          'WindowedApp'
    cppdialect    'C++20'

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
        '../../utils/*/include',
        '../../utils/*/source',
    }

    links
    {
        LibraryPaths
    }

    postbuildcommands
    {
        'powershell -ExecutionPolicy Bypass -File "../../utils/premake5/CreateExecutableShortcut.ps1" -projectFolder "../../" -executablePath $(TARGETPATH) -projectName ' .. WorkspaceName .. ' -WindowStyle Hidden',
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