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
        
        $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
        foreach ($sourceFile in $sourceFiles) {
            $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name
            
            
            if (-Not (Test-Path $destinationFilePath -PathType Leaf)) {
                Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
                Write-Host "Copying binary: "$sourceFile.Name
            }
        }

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

function Build-Vulkan {

}

function Compare-File {
    param (
        [string]$sourceFilePath,
        [string]$destinationFilePath
    )

    $sourceContent = Get-Content -Path $sourceFilePath -Raw
    $destinationContent = Get-Content -Path $destinationFilePath -Raw

    if ($sourceContent -ne $destinationContent) {
        Copy-Item -Path $sourceFilePath -Destination $destinationFilePath
        Build-Vulkan

        $fileInfo = Get-Item -LiteralPath $sourceFilePath
        Write-Host "Overwriting file: "$fileInfo.Name
    }
}

function Build-Shaders {
    $sourceFolder = "$projectFolder\source\shaders"
    $destinationFolder = "$projectFolder\game\binaries\shaders"

    if (-Not (Test-Path $destinationFolder)) {
        New-Item -ItemType Directory -Path $destinationFolder -Force
    }

    $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
    foreach ($sourceFile in $sourceFiles) {
        $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name
        
        if (Test-Path $destinationFilePath -PathType Leaf) {
            Compare-File -sourceFilePath $sourceFile.FullName -destinationFilePath $destinationFilePath
        }
        else {
            Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
            Build-Vulkan
            Write-Host "Copying shader: "$sourceFile.Name
        }
    }
}

Copy-Binaries -configuration $configuration
Build-Shaders