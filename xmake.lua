PROJECT_NAME = "DragonForge-Engine"

set_project( PROJECT_NAME )

if is_plat( "windows" ) then
    if is_mode( "Debug" ) then
        set_runtimes( "MTd" )
    else
        set_runtimes( "MT" )
    end
elseif is_plat( "linux" ) then
    set_runtimes( "stdc++_static" )
end

-- add_repositories( "overrides xmake-repo" )

includes( "xmake/init.lua" )
init_requires()

includes( "source/shaders" )
includes( "source/engine" )
includes( "source/application" )