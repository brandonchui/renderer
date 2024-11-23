@echo off
setlocal enabledelayedexpansion

echo Starting build process...

:: Configure step
echo Configuring project with CMake...
cmake -G Ninja -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
if errorlevel 1 (
    echo CMake configuration failed
    pause
    exit /b 1
)

:: Build step
echo Building project with Ninja...
ninja -C build
if errorlevel 1 (
    echo Build failed
    pause
    exit /b 1
)

:: Run step
echo Running the executable...
.\build\bin\renderer.exe
if errorlevel 1 (
    echo Program execution failed
    pause
    exit /b 1
)

echo Build process completed successfully
pause
