workspace( path.getname( os.getcwd() ) )
    startproject  'application'
    architecture  'x64'
    staticruntime 'on'

    configurations
    {
        'Debug',
        'Release',
    }

    flags
    {
        'MultiProcessorCompile',
    }

OutputDir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'

group 'libraries'
    include 'utils'

group 'source'
    include 'source/application'