PROJECT_NAME = "DragonForge-Engine"

set_project( PROJECT_NAME )

if not is_mode( "Debug", "Development" ) then
    set_runtimes( "MT" )
else
    set_runtimes( "MTd" )
end

includes( "xmake/platforms.lua" )
init_platforms()

includes( "source/shaders" )
includes( "source/engine" )
includes( "source/application" )