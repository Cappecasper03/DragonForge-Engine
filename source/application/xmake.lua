target( "application" )
    set_kind( "binary" )
    add_deps( "engine" )

    add_headerfiles( "**.h" )
    add_files( "**.cpp" )
    add_includedirs( "../" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    init_target()

    if is_plat( "windows" ) then
        add_files( "main/executable_icon.rc" )
        add_filegroups( "Resources", { rootdir = "resources" } )
    end
target_end()