WorkspaceDirectory = os.getcwd()
WorkspaceName      = path.getname( WorkspaceDirectory )

workspace ( WorkspaceName )
    startproject  'application'
    architecture  'x64'

    externalwarnings 'Off'

    configurations
    {
        'Debug',
        'Profiling',
        'Release',
    }

    platforms
    {
        'Windows',
    }

    flags
    {
        'MultiProcessorCompile',
        'FatalWarnings',
    }

    filter 'configurations:Debug'
        optimize 'Off'
        symbols  'Full'
        warnings 'Extra'
        runtime  'Debug'

        defines
        {
            'DEBUG',
        }

    filter 'configurations:Release'
        optimize 'Speed'
        symbols  'Off'
        runtime  'Release'

        flags
        {
            'LinkTimeOptimization',
        }

        defines
        {
            'RELEASE',
            'NDEBUG',
        }

filter{}

OutputDir = '%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}'
Libraries = {}

group 'libraries'
    include 'libraries'

group 'source'
    include 'source/application'