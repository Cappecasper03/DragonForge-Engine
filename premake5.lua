WorkspaceName = path.getname( os.getcwd() )

workspace ( WorkspaceName )
    startproject  'application'
    architecture  'x64'

    configurations
    {
        'Debug',
        'Release',
    }

    flags
    {
        'MultiProcessorCompile',
        'FatalWarnings',
    }

OutputDir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'

group 'libraries'
    include 'utils'

group 'source'
    include 'source/application'