target( "engine" )
    set_kind( "static" )
    add_deps( "shaders" )

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )

    add_headerfiles( "**.h", "**.inl" )
    add_files( "**.cpp" )
    
    add_includedirs( "../", "./" )

    init_target()
target_end()