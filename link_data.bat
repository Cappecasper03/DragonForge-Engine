@echo off

if exist .\build\debug\data ( del /q .\build\debug\data )
mklink /j .\build\debug\data .\data

if exist .\build\release\data ( del /q .\build\release\data )
mklink /j .\build\release\data .\data

if exist .\build\final\data ( del /q .\build\final\data )
mklink /j .\build\final\data .\data

if exist .\build\vs\data ( del /q .\build\vs\data )
mklink /j .\build\vs\data .\data

echo "\|/ Deleted itself, not a problem \|/ "
del /q .\link_data.bat