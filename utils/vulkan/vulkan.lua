filter 'files:../../source/shaders/vulkan/**'
    buildmessage ( 'Vulkan: %{file.basename}.spv' )
    buildcommands( 'glslc.exe "%{file.path}" -o "../../game/binaries/shaders/vulkan/%{file.basename}.spv"' )
    buildoutputs ( '../../game/binaries/shaders/vulkan/%{file.basename}.spv' )