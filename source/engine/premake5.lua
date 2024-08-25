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
    }

    filter 'configurations:Debug'
        targetname ( 'engined' )
        optimize   'Off'
        symbols    'Full'
        warnings   'Extra'
        runtime    'Debug'

        defines
        {
            'DEBUG',
        }

    filter 'configurations:Profiling'
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

    filter 'files:../../utils/*/include/**'
        flags
        {
            'ExcludeFromBuild',
        }