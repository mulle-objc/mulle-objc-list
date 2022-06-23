@ECHO OFF
REM need EnableDelayedExpansion to get !xxx! expansion which is needed %xx% is expanded during parse!
REM SETLOCAL EnableDelayedExpansion
REM "C:\Program Files\Git\usr\bin\sh.exe" should be replaced by install.sh or so

rem https://stackoverflow.com/questions/2323292/assign-output-of-a-program-to-a-variable-using-a-ms-batch-file
rem careful, trailing whitespace after =%%i is gobbled up by BAT
for /f "tokens=*" %%i in ( 'wsl.exe -e wslpath -u "%CD%"' ) do set wslpwd=%%i
for /f "tokens=*" %%i in ( 'wsl.exe -e wslpath -u "%~dp0\mulle-objc-list"' ) do set toolpath=%%i

rem omit -e so we get at least the standard path
wsl.exe --cd "%wslpwd%" "%toolpath%" --wsl %*

rem bash.exe "%~dp0\mulle-objc-list" %*

