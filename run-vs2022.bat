@REM Command to build EmberLily and a test application


CALL .\configure-vs2022.bat && cd ..
CALL .\build-vs2022.bat
@REM Abort building if there was an error building the library
if NOT %errorLevel% == 0 echo "Error building the engine lib" && exit /b
CALL cd ..

@REM  Configure the test application build
CALL cd sampleapp
CALL cmake -B build/vs2022 -G "Visual Studio 17 2022" -A x64

CALL cmake --build build/vs2022
@REM About running the previously build exe it building fails
if NOT %errorLevel% == 0 echo "Error building the sample app" && exit /b

CALL cd build/vs2022/Debug
.\SampleApp.exe
