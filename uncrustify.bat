@echo off

set UNCRUSTIFY=c:\schestde\tools\uncrustify\uncrustify.exe 

rem generate file list
dir /b /s src | find ".cpp" > files.lst
dir /b /s src | find ".h" >> files.lst

rem run uncrustify
%UNCRUSTIFY% -c superterm.cfg --replace --no-backup -F files.lst

rem cleanup
del /f files.lst

rem <stefan@scheler.com>
