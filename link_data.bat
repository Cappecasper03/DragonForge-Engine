@echo off

set data_folder=./data
set debug_folder=./build/debug
set release_folder=./build/release
set final_folder=./build/final
set vs_folder=./build/vs

REM Create the data folder if it doesn't exist
if not exist %data_folder% ( 
    mkdir %data_folder%
) 

REM Create the debug folder and create a symbolic link to the data folder if it doesn't exist
if not exist %debug_folder% (
    mkdir %debug_folder%
)
if not exist %debug_folder%/data ( 
    mklink /j %debug_folder%/data %data_folder%
)

REM Create the release folder and create a symbolic link to the data folder if it doesn't exist
if not exist %release_folder% (
    mkdir %release_folder%
)
if not exist %release_folder%/data ( 
    mklink /j %release_folder%/data %data_folder%
)

REM Create the final folder and create a symbolic link to the data folder if it doesn't exist
if not exist %final_folder% (
    mkdir %final_folder%
)
if not exist %final_folder%/data ( 
    mklink /j %final_folder%/data %data_folder%
)

REM Create the vs folder and create a symbolic link to the data folder if it doesn't exist
if not exist %vs_folder% (
    mkdir %vs_folder%
)
if not exist %vs_folder%/data ( 
    mklink /j %vs_folder%/data %data_folder%
)
