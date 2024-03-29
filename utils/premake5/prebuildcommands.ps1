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
        elseif ($configuration -eq "Release" -or "Profiling") {
            $sourceFolder = "$projectFolder\utils\$library\binaries\release"
        }
        else {
            break
        }
        $destinationFolder = "$projectFolder\game\binaries"
        
        if (-Not (Test-Path $sourceFolder)) {
            continue
        }

        # Create the destination folder structure if it isn't found
        if (-Not (Test-Path $destinationFolder)) {
            New-Item -ItemType Directory -Path $destinationFolder -Force
        }
        
        # Loop over every file in the source folder
        $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
        foreach ($sourceFile in $sourceFiles) {
            # Build the corresponding destination file path
            $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name
            
            
            if (-Not (Test-Path $destinationFilePath -PathType Leaf)) {
                # Copy the source file to the destination folder
                Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
                Write-Host "Copying binary: "$sourceFile.Name
            }
        }
    }
}

function Compare-File {
    param (
        [string]$sourceFilePath,
        [string]$destinationFilePath
    )

    # Compare file contents
    $sourceContent = Get-Content -Path $sourceFilePath -Raw
    $destinationContent = Get-Content -Path $destinationFilePath -Raw

    if ($sourceContent -ne $destinationContent) {
        # Copy the source file to the destination folder
        Copy-Item -Path $sourceFilePath -Destination $destinationFilePath

        $fileInfo = Get-Item -LiteralPath $sourceFilePath
        Write-Host "Overwriting file: "$fileInfo.Name
    }
}

function Build-Shaders {
    $sourceFolder = "$projectFolder\source\shaders"
    $destinationFolder = "$projectFolder\game\binaries\shaders"

    # Create the destination folder structure if it isn't found
    if (-Not (Test-Path $destinationFolder)) {
        New-Item -ItemType Directory -Path $destinationFolder -Force
    }
    
    # Loop over every file in the source folder
    $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
    foreach ($sourceFile in $sourceFiles) {
        # Build the corresponding destination file path
        $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name
        
        if (Test-Path $destinationFilePath -PathType Leaf) {
            # Call the function to compare the current file
            Compare-File -sourceFilePath $sourceFile.FullName -destinationFilePath $destinationFilePath
        }
        else {
            # Copy the source file to the destination folder
            Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
            Write-Host "Copying shader: "$sourceFile.Name
        }
    }
}

Copy-Binaries -configuration $configuration
Build-Shaders