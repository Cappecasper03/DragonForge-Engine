$current_folder = Split-Path -Leaf $PWD.Path
$settings_dir = "utils/premake5"
$workspace_name = $current_folder
$project_name = $current_folder

# Check if the new workspace and project names are already present in the settings file
$settings_content = Get-Content "$settings_dir/settings.lua"
$workspace_name_exists = $settings_content -match "workspace_name\s*=\s*['""]$workspace_name['""]"
$project_name_exists = $settings_content -match "project_name\s*=\s*['""]$project_name['""]"

# If the new workspace or project names are not already present, update them in the settings file
if (-not $workspace_name_exists) {
    $settings_content = $settings_content -replace "workspace_name\s*=\s*['""][^'""]*['""]", "workspace_name = '$workspace_name'"
}

if (-not $project_name_exists) {
    $settings_content = $settings_content -replace "project_name\s*=\s*['""][^'""]*['""]", "project_name = '$project_name'"
}

# Write the updated settings file
Set-Content "$settings_dir/settings.lua" $settings_content

# Run the Premake executable with the premake5.lua script
& "$settings_dir/premake5.exe" vs2022
