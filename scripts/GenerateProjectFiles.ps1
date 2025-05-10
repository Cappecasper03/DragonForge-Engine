Set-Location ..
& xmake repo -u
& xmake f -y -p "windows" -a "x64" -c
& xmake project -k vsxmake -y -m "Debug,Release,Profiling" ./build

if( $LASTEXITCODE -lt 0 )
{
    Set-Location scripts
    write-host "Press any key to continue..."
    [void][System.Console]::ReadKey($true)
}
else {
    Get-ChildItem "$(Get-Item .)\build\vsxmake2022" -Filter *.sln |
    ForEach-Object {
        write-host ($_.BaseName)
        $WshShell = New-Object -COMObject WScript.Shell
        $Shortcut = $WshShell.CreateShortcut("$(Get-Item .)\$($_.BaseName).sln.lnk")
        $Shortcut.TargetPath = "$(Get-Item .)\build\vsxmake2022\$($_.BaseName).sln"
        $Shortcut.Save()
    }
    Set-Location scripts
    & .\UpdateRiderXmakeEnvs.ps1
}