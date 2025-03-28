rule( "CompileSlangShader" )
    set_extensions( ".slang" )
    before_buildcmd_file( function ( target, batchcmds, sourcefile, opt )
        import( "lib.detect.find_tool" )

        -- local slangc = find_tool( "slangc" ) -- Should be used when it is possible
        local slangc = nil
        if target:is_plat( "windows" ) then
            slangc = path.join( os.getenv( "VULKAN_SDK" ), "Bin", "slangc.exe" )
        elseif target:is_plat( "linux" ) then
            slangc = path.join( os.getenv( "VULKAN_SDK" ), "bin", "slangc" )
        end
        assert( slangc, "slangc not found!" )
        
        -- slang to spv
        local outputdir  = path.join( target:targetdir(), "../shaders/vulkan" )
        local outputfile = path.join( outputdir, path.basename( sourcefile ) .. ".spv" )
        batchcmds:show_progress( opt.progress, "${color.build.object}generating.slang2spv %s", sourcefile )
        batchcmds:mkdir( outputdir )
        batchcmds:vrunv( slangc, { path( sourcefile ), "-o", path( outputfile ), "-profile", "glsl_460", "-target", "spirv", "-entry", "main", "-Wno-39029" } )

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