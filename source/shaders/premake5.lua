project 'shaders'
    kind 'Utility'

    targetdir ( '../../game/binaries/shaders' )
    location  ( '../../build/%{prj.name}' )
    objdir    ( '../../build/%{prj.name}/' .. OutputDir )

    files
    {
        '**',
    }

    filter{ 'files:opengl/**' }
        buildmessage ( 'OpenGL: %{file.name}' )
        buildcommands( 'copy "%{file.path}" "../../game/binaries/shaders/opengl/%{file.name}"' )
        buildoutputs ( '../../game/binaries/shaders/opengl/%{file.name}' )

    filter{ 'files:vulkan/**' }
        buildmessage ( 'Vulkan: %{file.basename}.spv' )
        buildcommands( 'glslc.exe "%{file.path}" -o "../../game/binaries/shaders/vulkan/%{file.basename}.spv"' )
        buildoutputs ( '../../game/binaries/shaders/vulkan/%{file.basename}.spv' )