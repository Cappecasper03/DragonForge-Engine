local libraries = os.matchdirs( path.join( os.getcwd(), '../*' ) )

for _, library_path in ipairs( libraries ) do
    if not string.find( library_path, 'premake5' ) then
        print( 'Adding library: ' .. path.getname( library_path ) )
    
        filter {}
            externalincludedirs  { library_path .. '/include', library_path .. '/source' }
            libdirs              { library_path .. '/lib' }
            files                { library_path .. '/source/**' }

        filter 'configurations:Debug'
            links { os.matchfiles( library_path .. '/lib/*d.lib' ) }

        filter 'configurations:Release or Profiling'
            links { os.matchfiles( library_path .. '/lib/*.lib' ) }

        if os.isfile( library_path .. '/custom.lua' ) then
            dofile( library_path .. '/custom.lua' )
        end
    end
end