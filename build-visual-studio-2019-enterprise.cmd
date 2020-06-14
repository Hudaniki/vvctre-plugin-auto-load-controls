@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
cl /LD plugin.cpp

del plugin.lib
del plugin.exp
del plugin.obj

echo Useless files deleted
pause
