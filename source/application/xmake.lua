target( "application" )
    set_kind( "binary" )
    add_deps( "engine" )

    add_headerfiles( "**.h" )
    add_files( "**.cpp" )
    add_includedirs( "../" )

    if is_plat( "windows" ) then
        add_files( "main/executable_icon.rc" )
        add_filegroups( "Resources", { rootdir = "resources" } )
    end

    init_target()
target_end()