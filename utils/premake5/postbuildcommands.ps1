param (
    [string]$projectFolder = "",
    [string]$executablePath = "",
    [string]$projectName = ""
)

$shortcutPath = "$projectFolder\game\$projectName.lnk"
    
$shell = New-Object -ComObject WScript.Shell
    
$shortcut = $shell.CreateShortcut($shortcutPath)
$shortcut.TargetPath = $executablePath
    
$shortcut.Save()
Write-Host "Created shortcut: "$projectName