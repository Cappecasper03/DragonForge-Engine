--[[ filter{ 'files:../../source/shaders/opengl/**' }
    buildmessage ( 'OpenGL: %{file.name}' )
    buildcommands( 'copy "%{file.path}" "../../game/binaries/shaders/opengl/%{file.name}"' )
    buildoutputs ( '../../game/binaries/shaders/opengl/%{file.name}' )

filter{ 'files:../../source/shaders/vulkan/**' }
    buildmessage ( 'Vulkan: %{file.basename}.spv' )
    buildcommands( 'glslc.exe "%{file.path}" -o "../../game/binaries/shaders/vulkan/%{file.basename}.spv"' )
    buildoutputs ( '../../game/binaries/shaders/vulkan/%{file.basename}.spv' ) ]]