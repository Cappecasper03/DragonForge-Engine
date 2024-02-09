param (
    [string]$projectFolder = "",
    [string]$executablePath = "",
    [string]$projectName = ""
)

# Specify the desired location for the shortcut
$shortcutPath = "$projectFolder\game\$projectName.lnk"
    
# Create a WScript.Shell object
$shell = New-Object -ComObject WScript.Shell
    
# Create a shortcut
$shortcut = $shell.CreateShortcut($shortcutPath)
$shortcut.TargetPath = $executablePath
    
# Save the shortcut
$shortcut.Save()
Write-Host "Created shortcut: "$projectName