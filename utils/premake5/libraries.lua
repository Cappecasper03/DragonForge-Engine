local files = os.matchfiles( path.join( ..., "libraries/*.lua" ) )

for _, file in ipairs( files ) do
    dofile( file )
end