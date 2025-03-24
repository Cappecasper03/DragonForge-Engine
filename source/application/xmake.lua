target( "application" )
    set_kind( "binary" )
    add_deps( "engine" )

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )

    add_headerfiles( "**.h" )
    add_files( "**.cpp" )
    add_includedirs( "../", "./" )

    add_files( "settings/executable_icon.rc" )
    add_filegroups( "Resources", { rootdir = "$(projectdir)" } )

    init_target()
target_end()