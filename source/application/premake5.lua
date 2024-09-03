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
        '../../libraries/*/include',
        '../../libraries/*/source',
    }

    libdirs
    {
        '../../build/lib'
    }

    links
    {
        Libraries,
        "engine",
    }

    filter 'configurations:Debug'
        targetname ( WorkspaceName .. '-debug' )

    filter 'configurations:Profiling'
        targetname ( WorkspaceName .. '-profiling' )

    filter 'configurations:Release'
        targetname ( WorkspaceName .. '-release' )

    filter 'platforms:Windows'
        postbuildcommands
        {
            'powershell -ExecutionPolicy Bypass -File "../../scripts/CreateExecutableShortcut.ps1" -projectFolder "../../" -executablePath $(TARGETPATH) -projectName ' .. WorkspaceName .. ' -WindowStyle Hidden',
        }