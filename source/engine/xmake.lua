target( "engine" )
    set_kind( "static" )
    add_deps( "shaders" )

    set_warnings( "extra" )

    add_headerfiles( "**.h", "**.inl" )
    add_files( "**.cpp" )
    
    add_includedirs( "../", "./" )

    init_target()
target_end()