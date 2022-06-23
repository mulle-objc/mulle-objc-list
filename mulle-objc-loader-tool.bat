@ECHO OFF

setlocal
setlocal EnableExtensions DisableDelayedExpansion

rem https://stackoverflow.com/questions/70926362/simple-batch-file-only-fails-when-called-from-nmake-exe
rem 32bit/64bit windows madness

set "SystemPath=%SystemRoot%\Sysnative"
if exist %SystemRoot%\System32\wsl.exe (
   set "SystemPath=%SystemRoot%\System32"
) 
      
if not exist %SystemPath%\wsl.exe (
   echo ERROR: Could not find wsl.exe! 
   exit /B 
) 

rem https://stackoverflow.com/questions/2323292/assign-output-of-a-program-to-a-variable-using-a-ms-batch-file
rem careful, trailing whitespace after =%%i is gobbled up by BAT, use ""

for /f "tokens=*" %%i in ( '%SystemPath%\wsl.exe -e wslpath -u "%CD%"' ) do set "wslpwd=%%i"
for /f "tokens=*" %%i in ( '%SystemPath%\wsl.exe -e wslpath -u "%~dp0\mulle-objc-loader-tool"' ) do set "toolpath=%%i"

rem omit -e so we get at least the standard path
%SystemPath%\wsl.exe --cd "%wslpwd%" "%toolpath%" -vvv -ld -ls --wsl %*
