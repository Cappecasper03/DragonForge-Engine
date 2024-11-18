Libraries = table.filter( os.matchdirs( '*' ), function( path ) return not path:match( 'premake' ) end )

for _, LibraryPath in ipairs( Libraries ) do
    local LibraryName = path.getname( LibraryPath )

    local file = io.open( LibraryName .. '/source/build.cpp', 'w' )
    file:write( '// Only needed for causing a build so a .lib is created' )

group 'libraries'
    project ( LibraryName )
        kind       'StaticLib'
        cppdialect 'C++Latest'

        targetname ( LibraryName .. '-lib' )
        targetdir  ( '../build/lib' )
        location   ( '../build/%{prj.name}' )
        objdir     ( '../build/%{prj.name}/' .. OutputDir )

        warnings 'Off'

        files
        {
            LibraryName .. '/include/**',
            LibraryName .. '/source/**',
            LibraryName .. '/lib/*.pdb',
        }

        includedirs
        {
            LibraryName .. '/include',
            LibraryName .. '/source',
        }

        libdirs
        {
            LibraryName .. '/lib',
        }

        filter 'configurations:Debug'
            targetname ( LibraryName .. '-libd' )

            files
            {
                LibraryName .. '/bin/*d.dll',
                LibraryName .. '/lib/*d.lib',
            }

            links
            {
                os.matchfiles( LibraryName .. '/lib/*d.lib' ),
            }

        filter 'configurations:Release'
            files
            {
                table.filter( os.matchfiles( LibraryName .. '/bin/*.dll' ), function( filename ) return not filename:match( "d%.dll$" ) end ),
                table.filter( os.matchfiles( LibraryName .. '/lib/*.lib' ), function( filename ) return not filename:match( "d%.lib$" ) end ),
            }

            links
            {
                table.filter( os.matchfiles( LibraryName .. '/lib/*.lib' ), function( filename ) return not filename:match( "d%.lib$" ) end )
            }

        filter 'files:*/include/**'
            flags
            {
                'ExcludeFromBuild',
            }

        filter 'files:*/bin/*'
            buildmessage ( 'Binary: %{file.name}' )
            buildcommands( 'copy "%{file.path}" "../game/binaries/%{file.name}"' )
            buildoutputs ( '../game/binaries/%{file.name}' )

        filter { 'files:*/lib/*.pdb' }
            buildmessage ( 'PDB: %{file.name}' )
            buildcommands( 'copy "%{file.path}" "../../build/lib/%{file.name}"' )
            buildoutputs ( '../build/lib/%{file.name}' )

        filter{}
            if os.isfile( LibraryName .. '/premake5.lua' ) then
                include ( LibraryName )
            end
end