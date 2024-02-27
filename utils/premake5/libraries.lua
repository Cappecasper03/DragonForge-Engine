local libraries = os.matchdirs( path.join( os.getcwd(), "../*" ) )

for _, library_path in ipairs( libraries ) do
    if not string.find( library_path, "premake5" ) then
        print( "Adding library: " .. path.getname( library_path ) )
    
        filter {}
            local lib_files = os.matchfiles( library_path .. "/lib/all/*.lib" )
            links { lib_files }
            externalincludedirs  { library_path .. "/include" }
            libdirs              { library_path .. "/lib" }
            files                { library_path .. "/source/**" }

        filter "configurations:Debug"
            local lib_files = os.matchfiles( library_path .. "/lib/debug/*.lib" )
            links { lib_files }

        filter "configurations:Release"
            local lib_files = os.matchfiles( library_path .. "/lib/release/*.lib" )
            links { lib_files }

        if os.isfile( library_path .. "/custom.lua" ) then
            dofile( library_path .. "/custom.lua" )
        end
    end
end