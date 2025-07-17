PROJECT_NAME = "DragonForge-Engine"

set_project( PROJECT_NAME )

-- add_repositories( "overrides xmake-repo" )

includes( "xmake/init.lua" )
init_requires()

includes( "source/shaders" )
includes( "source/engine" )
includes( "source/application" )