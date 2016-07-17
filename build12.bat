@echo OFF
call 	set PATH=%PATH%;"CMake\bin\"
call cmake -G "Visual Studio 12"
call cmake --build . --target ALL_BUILD --config Release
call cmake --build . --target ALL_BUILD --config Debug
start ImageBrowser.sln /D .
@echo ON
pause