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

function Build-VulkanShaders {
    $sourceFolder = "$projectFolder\source\shaders\vulkan"
    $destinationFolder = "$projectFolder\game\binaries\shaders\vulkan"
    $CompareFolder = "$projectFolder\build\vulkan\compiled"

    if (-Not (Test-Path $destinationFolder)) {
        New-Item -ItemType Directory -Path $destinationFolder -Force
    }

    if (-Not (Test-Path $CompareFolder)) {
        New-Item -ItemType Directory -Path $CompareFolder -Force
    }

    $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
    foreach ($sourceFile in $sourceFiles) {
        $CompareFilePath = Join-Path $CompareFolder $sourceFile.Name
        $vulkanFilePath = Join-Path -Path $destinationFolder -ChildPath "$sourceFile.spv"

        if (-Not (Test-Path $CompareFilePath -PathType Leaf) -or (Compare-Files -sourceFilePath $sourceFile.FullName -destinationFilePath $CompareFilePath)) {
            & "$projectFolder\utils\vulkan\glslc.exe" $sourceFile.FullName -o $vulkanFilePath
            Copy-Item -Path $sourceFile.FullName -Destination $CompareFilePath
            Write-Host "Compiling and copying vulkan shader: "$sourceFile.Name
        }
    }
}

# Returns true if they are different
function Compare-Files {
    param (
        [string]$sourceFilePath,
        [string]$destinationFilePath
    )

    $sourceContent = Get-Content -Path $sourceFilePath -Raw
    $destinationContent = Get-Content -Path $destinationFilePath -Raw

    return $sourceContent -ne $destinationContent
}

function Build-OpenGLShaders {
    $sourceFolder = "$projectFolder\source\shaders\opengl"
    $destinationFolder = "$projectFolder\game\binaries\shaders\opengl"

    if (-Not (Test-Path $destinationFolder)) {
        New-Item -ItemType Directory -Path $destinationFolder -Force
    }

    $sourceFiles = Get-ChildItem -Path $sourceFolder -File -Recurse
    foreach ($sourceFile in $sourceFiles) {
        $destinationFilePath = Join-Path $destinationFolder $sourceFile.Name

        if (-Not (Test-Path $destinationFilePath -PathType Leaf) -or (Compare-Files -sourceFilePath $sourceFile.FullName -destinationFilePath $destinationFilePath)) {
            Copy-Item -Path $sourceFile.FullName -Destination $destinationFilePath
            Write-Host "Copying opengl shader: "$sourceFile.Name
        }
    }
}

Copy-Binaries -configuration $configuration
Build-OpenGLShaders
Build-VulkanShaders