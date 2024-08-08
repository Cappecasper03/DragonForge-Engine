local libraries = os.matchdirs( path.join( os.getcwd(), '../*' ) )

for _, library_path in ipairs( libraries ) do
    if not string.find( library_path, 'premake5' ) then
        print( 'Library: ' .. path.getname( library_path ) )
    
        filter {}
            externalincludedirs  { library_path .. '/include', library_path .. '/source' }
            libdirs              { library_path .. '/lib' }
            files                {
                library_path .. '/include/**',
                library_path .. '/source/**'
            }

        filter 'configurations:Debug'
            links { os.matchfiles( library_path .. '/lib/*d.lib' ) }
            files {
                library_path .. '/lib/*d.*',
                library_path .. '/bin/*d.*',
            }

        filter 'configurations:Release or Profiling'
            links { table.filter( os.matchfiles( library_path .. '/lib/*.lib' ), function( filename ) return not filename:match( "d%.lib$" ) end ) }
            files {
                table.filter( os.matchfiles( library_path .. '/lib/*' ), function( filename ) return not filename:match( "d%.[^\\]*" ) end ),
                table.filter( os.matchfiles( library_path .. '/bin/*' ), function( filename ) return not filename:match( "d%.[^\\]*" ) end ),
            }

        for _, lua_file in ipairs( os.matchfiles( library_path .. '/*.lua' ) ) do
            dofile( lua_file )
        end
    end
end

filter { 'files:../**/include/**' }
    flags { 'ExcludeFromBuild' }

filter { 'files:../**/bin/*' }
    buildmessage ( 'Binary: %{file.name}' )
    buildcommands( 'copy "%{file.path}" "../../game/binaries/%{file.name}"' )
    buildoutputs ( '../../game/binaries/%{file.name}' )