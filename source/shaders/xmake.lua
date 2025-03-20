rule( "CompileOpenGL" )
    set_extensions(".vert", ".frag")
    on_build_file( function ( target, sourcefile, opt )
        local outputdir = path.join( target:targetdir(), "../shaders/opengl" )
        os.mkdir( outputdir )
        local outputfile = path.join( outputdir, path.filename( sourcefile ) )
        os.cp( sourcefile, outputfile )
    end )

target( "opengl_shaders" )
    set_kind "static"

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )

    add_rules( "CompileOpenGL" )
    add_files( "opengl/**.vert", "opengl/**.frag" )
    add_extrafiles( "opengl/**.vert", "opengl/**.frag" )

    target_platform()
target_end()

target( "vulkan_shaders" )
    set_kind "static"

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )

    add_rules( "utils.glsl2spv", { outputdir = "game/binaries/shaders/vulkan" } )
    add_files( "vulkan/**.vert", "vulkan/**.frag" )
    add_extrafiles( "vulkan/**.vert", "vulkan/**.frag" )

    target_platform()
target_end()