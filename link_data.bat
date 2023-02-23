@echo off

if not exist .\data ( mkdir .\data )

if exist .\build\debug\data ( del /q .\build\debug\data ) 
else ( mkdir .\build\debug\data )

if exist .\build\release\data ( del /q .\build\release\data ) 
else (mkdir .\build\release\data )

if exist .\build\final\data ( del /q .\build\final\data ) 
else ( mkdir .\build\final\data )

if exist .\build\vs\data ( del /q .\build\vs\data )
else( mkdir .\build\vs\data )

mklink /j .\build\debug\data .\data
mklink /j .\build\release\data .\data
mklink /j .\build\final\data .\data
mklink /j .\build\vs\data .\data