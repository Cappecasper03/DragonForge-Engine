rule( "CompileSlangShader" )
    set_extensions( ".slang" )
    on_build_file( function ( target, sourcefile, opt )
        local vulkan_sdk = os.getenv( "VULKAN_SDK" )
        if vulkan_sdk then
            local slangc = path.join( vulkan_sdk, "Bin", "slangc.exe" )

            if os.isfile( slangc ) then
                local outputdir = path.join( target:targetdir(), "../shaders/vulkan" )
                os.mkdir( outputdir )
                local outputfile = path.join( outputdir, path.basename( sourcefile ) .. ".spv" )
                os.execv( slangc, { sourcefile, "-profile", "glsl_460", "-target", "spirv", "-o", outputfile, "-entry", "main" } )

                local outputdir = path.join( target:targetdir(), "../shaders/opengl" )
                os.mkdir( outputdir )
                local outputfile = path.join( outputdir, path.basename( sourcefile ) .. ".glsl" )
                os.execv( slangc, { sourcefile, "-profile", "glsl_460", "-target", "glsl", "-o", outputfile, "-entry", "main" } )
            end
        end
    end )

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