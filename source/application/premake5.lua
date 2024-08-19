project 'application'
    kind       'WindowedApp'
    cppdialect 'C++20'

    targetdir ( '../../game/binaries' )
    location  ( '../../build/%{prj.name}' )
    objdir    ( '../../build/%{prj.name}/' .. OutputDir )

    files
    {
        '*/**',
    }