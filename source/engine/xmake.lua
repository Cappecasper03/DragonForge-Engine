target( "engine" )
    set_kind( "static" )
    add_deps( "shaders" )

    set_warnings( "extra" )

    add_headerfiles( "**.h", "**.inl" )
    add_files( "**.cpp" )
    
    add_includedirs( "../", "./" )

    init_target()
    
    if is_plat( "windows" ) then
        add_packages( "tracy-server" )
    end
target_end()