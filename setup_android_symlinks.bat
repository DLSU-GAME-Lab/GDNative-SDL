@echo off
cd /d %~dp0android-project\app\jni

echo Removing old directories...

if exist src (rmdir /S /Q src)
if exist include (rmdir /S /Q include)

echo Creating symbolic links...
mklink /D src ..\..\..\SDLTest\src
mklink /D include ..\..\..\SDLTest\include

echo Done.
pause