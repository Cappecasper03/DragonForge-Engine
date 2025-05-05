target( "core" )
    set_kind( "static" )

    set_warnings( "extra" )

    -- Engine core components
    add_headerfiles( "**.h", "**.inl" )
    add_files( "**.cpp" )
    
    add_includedirs( "../", "./" )

    init_target()
target_end()