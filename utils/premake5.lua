LibraryPaths = os.matchdirs( '*' )

for _, library_path in ipairs( LibraryPaths ) do
    print( library_path )
end