@echo off

@REM Command to configure EmberLily engine for building using the Visual Studio 2022
@REM generator (after dependencies have been installed)

cd emberlily
cmake -B build/vs2022 -G "Visual Studio 17 2022" -A x64 ^
-DCMAKE_PREFIX_PATH=%cd%/emberlily/third-party

@REM cd ..
@REM cd third-party/spdlog
@REM cmake -B build -G "Visual Studio 17 2022" -A x64 
@REM cmake --build build (or something like this)