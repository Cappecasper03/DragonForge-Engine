target( "application" )
    set_kind( "binary" )
    add_deps( "engine" )

    set_warnings( "extra" )

    add_headerfiles( "**.h" )
    add_files( "**.cpp" )
    add_includedirs( "../", "./" )

    if is_plat( "windows" ) then
        add_files( "settings/executable_icon.rc" )
        add_filegroups( "Resources", { rootdir = "settings" } )
    end

    init_target()
target_end()