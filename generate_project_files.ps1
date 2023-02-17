$current_folder = Split-Path -Leaf $PWD.Path
$settings_dir = "utils/premake5"
$workspace_name = $current_folder
$project_name = $current_folder

# Update the workspace and project names in the settings.lua file
(Get-Content "$settings_dir/settings.lua") -replace "workspace_name = `".*`"", "workspace_name = `"$workspace_name`"" | Set-Content "$settings_dir/settings.lua"
(Get-Content "$settings_dir/settings.lua") -replace "project_name = `".*`"", "project_name = `"$project_name`"" | Set-Content "$settings_dir/settings.lua"

# Run the Premake executable with the premake5.lua script
& "$settings_dir/premake5.exe" vs2022
