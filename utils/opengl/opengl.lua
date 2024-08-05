filter {}
    links { 'opengl32.lib' }

filter 'files:../../source/shaders/opengl/**'
    buildmessage ( 'OpenGL: %{file.name}' )
    buildcommands( 'copy "%{file.path}" "../../game/binaries/shaders/opengl/%{file.name}"' )
    buildoutputs ( '../../game/binaries/shaders/opengl/%{file.name}' )
