@echo off

set infile=%1
set outfile=%2
cd "%~dp0..\..\bin"

sam-ba

if ERRORLEVEL 1 goto sam-ba_notfound
sam-ba -u gen_image:sama7d65:%infile%:%outfile%::
goto scriptend

:sam-ba_notfound
echo SAM-BA instalation not found! Can't generate %outfile%!

:scriptend
cd "%~dp0"
exit /b 0