Set-Location ../..
& xmake repo -u
& xmake f -y -p "windows" -a "x64" -m "Shipping" -c
& xmake -r

if( $LASTEXITCODE -lt 0 )
{
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
}
Set-Location scripts/windows