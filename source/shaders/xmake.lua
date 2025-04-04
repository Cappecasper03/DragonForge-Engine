rule( "CompileSlangShader" )
    set_extensions( ".slang" )
    before_buildcmd_file( function ( target, batchcmds, sourcefile, opt )
        import("core.project.project")

        local slang = project.required_package( "slang" )
        local slangc = path.join( slang:installdir(), "bin/slangc", target:is_plat( "windows" ) and ".exe" or "" )
        assert( slangc, "slangc not found!" )
        
        -- slang to spv
        local outputdir  = path.join( target:targetdir(), "../shaders/vulkan" )
        local outputfile = path.join( outputdir, path.basename( sourcefile ) .. ".spv" )
        batchcmds:show_progress( opt.progress, "${color.build.object}generating.slang2spv %s", path.basename( sourcefile ) )
        batchcmds:mkdir( outputdir )
        batchcmds:vrunv( slangc, { path( sourcefile ), "-o", path( outputfile ), "-profile", "glsl_460", "-target", "spirv", "-entry", "main", "-Wno-39029", "-Wno-39013" } )

        -- add deps
        batchcmds:add_depfiles( sourcefile )
        batchcmds:set_depmtime( os.mtime( outputfile ) )
        batchcmds:set_depcache( target:dependfile( outputfile ) )
    end)

rule( "CompileOpenGL" )
    set_extensions( ".glsl" )
    before_buildcmd_file( function ( target, batchcmds, sourcefile, opt )
        -- copy glsl
        local outputdir  = path.join( target:targetdir(), "../shaders/opengl" )
        local outputfile = path.join( outputdir, path.basename( sourcefile ) .. ".glsl" )
        batchcmds:show_progress( opt.progress, "${color.build.object}copying.glsl %s", sourcefile )
        batchcmds:mkdir( outputdir )
        batchcmds:cp( path( sourcefile ), path( outputfile ) )

        -- add deps
        batchcmds:add_depfiles( sourcefile )
        batchcmds:set_depmtime( os.mtime( outputfile ) )
        batchcmds:set_depcache( target:dependfile( outputfile ) )
    end )

target( "shaders" )
    set_kind "static"

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )

    add_rules( "CompileSlangShader", "CompileOpenGL" )
    add_files( "**.slang", "**.glsl" )
    add_extrafiles( "**.slang", "**.glsl" )

    init_target()
target_end()