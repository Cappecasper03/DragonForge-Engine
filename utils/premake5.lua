LibraryPaths = table.filter( os.matchdirs( '*' ), function( path ) return not path:match( 'premake' ) end )

for _, LibraryPath in ipairs( LibraryPaths ) do
    local LibraryName = path.getname( LibraryPath )

    local file = io.open( LibraryName .. '/source/build.cpp', 'w' )
    file:write( '// Only needed for causing a build so a .lib is created' )

    project ( LibraryName )
        kind          'StaticLib'
        cppdialect    'C++Latest'

        targetname ( LibraryName )
        targetdir  ( '../build/lib' )
        location   ( '../build/%{prj.name}' )
        objdir     ( '../build/%{prj.name}/' .. OutputDir )

        warnings 'Off'

        files
        {
            LibraryName .. '/include/**',
            LibraryName .. '/source/**',
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
            targetname ( LibraryName .. 'd' )
            optimize   'Off'
            symbols    'Full'
            runtime    'Debug'

            defines
            {
                'DEBUG',
            }

            links
            {
                os.matchfiles( LibraryName .. '/lib/*d.lib' ),
            }

        filter 'configurations:Release or Profiling'
            optimize 'Speed'
            symbols  'Off'
            runtime  'Release'

            flags
            {
                'LinkTimeOptimization',
            }

            defines
            {
                'RELEASE',
                'NDEBUG',
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
            buildcommands( 'copy "%{file.path}" "../../game/binaries/%{file.name}"' )
            buildoutputs ( '../../game/binaries/%{file.name}' )

        filter{}
            if os.isfile( LibraryName .. '/premake5.lua' ) then
                include ( LibraryName )
            end
end