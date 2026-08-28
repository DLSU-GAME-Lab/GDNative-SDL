@echo off
setlocal

set "ROOT=%~dp0"
set "JNI=%ROOT%android-project\app\jni"

cd /d "%JNI%"

echo Removing old directories...

if exist "src" rmdir /S /Q "src"
if exist "include" rmdir /S /Q "include"

echo Creating symbolic links...

mklink /D "src" "%ROOT%SDLTest\src"
mklink /D "include" "%ROOT%SDLTest\include"

echo.
echo Done.
pause