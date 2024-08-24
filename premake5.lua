WorkspaceName = path.getname( os.getcwd() )

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

    defines
    {
        'VULKAN_HPP_NO_EXCEPTIONS',
        'VULKAN_HPP_DISPATCH_LOADER_DYNAMIC',
        'VULKAN_NO_PROTOTYPES',
        'VULKAN_HPP_NO_NODISCARD_WARNINGS',
    }

    flags
    {
        'MultiProcessorCompile',
        'FatalWarnings',
    }

OutputDir = '%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}'

group 'utils'
    include 'utils'

group 'source'
    include 'source/shaders'
    include 'source/engine'
    include 'source/application'