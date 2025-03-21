rule( "CompileSlangShader" )
    set_extensions( ".slang" )
    before_buildcmd_file( function ( target, batchcmds, sourcefile, opt )
        import( "lib.detect.find_tool" )

        -- local slangc = find_tool( "slangc" ) -- Should be used when it is possible
        local slangc = path.join( os.getenv( "VULKAN_SDK" ), "Bin", "slangc.exe" )
        assert( slangc, "slangc not found!" )
        
        -- slang to spv
        local spvoutputdir  = path.join( target:targetdir(), "../shaders/vulkan" )
        local spvoutputfile = path.join( spvoutputdir, path.basename( sourcefile ) .. ".spv" )
        batchcmds:show_progress( opt.progress, "${color.build.object}generating.slang2spv %s", sourcefile )
        batchcmds:mkdir( spvoutputdir )
        batchcmds:vrunv( slangc, { path( sourcefile ), "-o", path( spvoutputfile ), "-profile", "glsl_460", "-target", "spirv", "-entry", "main" } )

        -- slang to glsl
        local glsloutputdir  = path.join( target:targetdir(), "../shaders/opengl" )
        local glsloutputfile = path.join( glsloutputdir, path.basename( sourcefile ) .. ".glsl" )
        batchcmds:show_progress( opt.progress, "${color.build.object}generating.slang2glsl %s", sourcefile )
        batchcmds:mkdir( glsloutputdir )
        batchcmds:vrunv( slangc, { path( sourcefile ), "-o", path( glsloutputfile ), "-profile", "glsl_460", "-target", "glsl", "-entry", "main" } )

        -- add deps
        batchcmds:add_depfiles( sourcefile )
        batchcmds:set_depmtime( os.mtime( spvoutputfile ) )
        batchcmds:set_depmtime( os.mtime( glsloutputfile ) )
        batchcmds:set_depcache( target:dependfile( spvoutputfile ) )
        batchcmds:set_depcache( target:dependfile( glsloutputfile ) )
    end)

target( "shaders" )
    set_kind "static"

    set_warnings( "extra" )

    set_basename( PROJECT_NAME .. "_$(mode)_$(arch)" )

    set_targetdir( "../../game/binaries/$(plat)" )
    set_objectdir( "../../build/obj" )

    add_rules( "CompileSlangShader" )
    add_files( "**.slang" )
    add_extrafiles( "**.slang" )

    target_platform()
target_end()