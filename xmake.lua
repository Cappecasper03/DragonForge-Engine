PROJECT_NAME = "DragonForge-Engine"

set_project( PROJECT_NAME )

if is_mode( "Debug", "Development" ) then
    set_runtimes( "MTd" )
else
    set_runtimes( "MT" )
end

includes( "xmake/init.lua" )
init_requires()

includes( "source/shaders" )
includes( "source/engine" )
includes( "source/application" )