project 'engine'
    kind       'StaticLib'
    cppdialect 'C++Latest'

    targetname ( 'engine' )
    targetdir ( '../../build/lib' )
    location  ( '../../build/%{prj.name}' )
    objdir    ( '../../build/%{prj.name}/' .. OutputDir )

    files
    {
        '*/**',
    }

    includedirs
    {
        '../../source',
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
    }

    filter 'files:../../utils/*/include/**'
        flags
        {
            'ExcludeFromBuild',
        }