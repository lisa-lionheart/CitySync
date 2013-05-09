@echo off
set NODE_DIR=C:\Program Files\nodejs
set PATH=%PATH%;%CD%\bin

echo Using node JS at %NODE_DIR% ...
call "%NODE_DIR%\nodevars.bat"

"%NODE_DIR%\node.exe" js/CitySync.js

pause