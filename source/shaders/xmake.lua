rule( "CompileSlang" )
    set_extensions( ".slang" )
    before_buildcmd_file( function ( target, batchcmds, sourcefile, opt )
        -- copy slang
        local outputdir  = path.join( target:targetdir(), "../shaders" )
        local outputfile = path.join( outputdir, path.basename( sourcefile ) .. ".slang" )
        batchcmds:show_progress( opt.progress, "${color.build.object}copying.slang %s", path.basename( sourcefile ) )
        batchcmds:mkdir( outputdir )
        batchcmds:cp( path( sourcefile ), path( outputfile ) )

        -- add deps
        batchcmds:add_depfiles( sourcefile )
        batchcmds:set_depmtime( os.mtime( outputfile ) )
        batchcmds:set_depcache( target:dependfile( outputfile ) )
    end)

target( "shaders" )
    set_kind "static"

    set_warnings( "extra" )

    add_rules( "CompileSlang" )
    add_files( "**.slang" )
    add_extrafiles( "**.slang" )

    init_target()
target_end()