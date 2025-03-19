PROJECT_NAME = "DragonForge-Engine"

set_project( PROJECT_NAME )

if not is_mode( "Debug" ) then
    set_runtimes( "MT" )
else
    set_runtimes( "MTd" )
end

includes( "xmake/platforms.lua" )
init_platforms()

includes( "source/application" )
includes( "source/engine" )