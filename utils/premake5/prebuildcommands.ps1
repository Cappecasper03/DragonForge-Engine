param (
    [string]$projectFolder = "",
    [string]$configuration = ""
)

function Copy-Binaries {
    $libraries = Get-ChildItem -Directory "$projectFolder\utils\"

    foreach ($library in $libraries) {
        if ($configuration -eq "Debug") {
            $sourceFolder = "$projectFolder\utils\$library\binaries\debug"
        }
        elseif ($configuration -eq "Release" -or $configuration -eq "Profiling") {
            $sourceFolder = "$projectFolder\utils\$library\binaries\release"
        }
        else {
            break
        }
        $destinationFolder = "$projectFolder\game\binaries"

        if (-Not (Test-Path $destinationFolder)) {
            New-Item -ItemType Directory -Path $destinationFolder -Force
        }
        
        if (Test-Path $sourceFolder) {
            $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
            foreach ($sourceFile in $sourceFiles) {
                $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name
                
                
                if (-Not (Test-Path $destinationFilePath -PathType Leaf)) {
                    Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
                    Write-Host "Copying binary: "$sourceFile.Name
                }
            }
        }
            
        if (Test-Path "$projectFolder\utils\$library\binaries\all") {
            $sourceFiles = Get-ChildItem -Path "$projectFolder\utils\$library\binaries\all" -File -Recurse
            foreach ($sourceFile in $sourceFiles) {
                $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name
                
                
                if (-Not (Test-Path $destinationFilePath -PathType Leaf)) {
                    Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
                    Write-Host "Copying binary: "$sourceFile.Name
                }
            }
        }
    }
}

Copy-Binaries -configuration $configuration