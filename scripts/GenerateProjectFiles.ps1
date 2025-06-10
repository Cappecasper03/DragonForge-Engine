Set-Location ..
& xmake repo -u
& xmake f -y -p "windows" -a "x64" -c --policies=generator.vsxmake.root_sln
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