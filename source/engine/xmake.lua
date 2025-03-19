target( "engine" )
    set_kind "static"

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir "../../game/binaries/$(plat)"
    set_objectdir "../../build/obj"

    add_headerfiles( "**.h" )
    add_files( "**.cpp" )
    
    add_includedirs( 
        "../",
        "./" )

    target_platform()
target_end()