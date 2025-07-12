Set-Location ..
& xmake repo -u
& xmake f -c -p "windows" -a "x64" -y --policies=generator.vsxmake.root_sln
& xmake project -k vsxmake -y -m "Debug,Release,Profiling"
Set-Location scripts

if( $LASTEXITCODE -lt 0 )
{
    write-host "Press any key to continue..."
    [void][System.Console]::ReadKey($true)
}
else
{
   & .\UpdateRiderXmakeEnvs.ps1
}