local libraries = os.matchdirs( path.join( os.getcwd(), '../*' ) )

for _, library_path in ipairs( libraries ) do
    if not string.find( library_path, 'premake5' ) then
        print( 'Adding library: ' .. path.getname( library_path ) )
    
        filter {}
            externalincludedirs  { library_path .. '/include', library_path .. '/source' }
            libdirs              { library_path .. '/lib' }
            files                {
                library_path .. '/include/**',
                library_path .. '/source/**',
                library_path .. '/lib/**',
                library_path .. '/bin/**',
            }

        filter 'configurations:Debug'
            links { os.matchfiles( library_path .. '/lib/*d.lib' ) }

        filter 'configurations:Release or Profiling'
            links { os.matchfiles( library_path .. '/lib/*.lib' ) }

        for _, lua_file in ipairs( os.matchfiles( library_path .. '/*.lua' ) ) do
            dofile( lua_file )
        end
    end
end

filter { 'files:../**/bin/*d.dll', 'configurations:Debug' }
    buildmessage ( 'DLL: %{file.name}' )
    buildcommands( 'copy "%{file.path}" "../../game/binaries/%{file.name}"' )
    buildoutputs ( '../../game/binaries/%{file.name}' )

filter { 'files:../**/bin/*.dll', 'configurations:Release or Profiling' }
    buildmessage ( 'DLL: %{file.name}' )
    buildcommands( 'copy "%{file.path}" "../../game/binaries/%{file.name}"' )
    buildoutputs ( '../../game/binaries/%{file.name}' )

filter 'files:../**/include/**'
    flags { 'ExcludeFromBuild' }