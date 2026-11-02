@echo off
setlocal
rem Builds into .dist\app. Needs QT_ROOT (Qt MSVC kit), VS 2022 C++ tools and Git. See README.

cd /d "%~dp0"

if not defined QT_ROOT (
    echo QT_ROOT is not set. Example: set QT_ROOT=C:\Qt\6.8.0\msvc2022_64
    exit /b 1
)
if not exist "%QT_ROOT%\bin\windeployqt.exe" (
    echo "%QT_ROOT%\bin\windeployqt.exe" not found. Check QT_ROOT.
    exit /b 1
)

pushd "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer" || exit /b 1
for /f "delims=" %%i in ('.\vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set "VSDIR=%%i"
popd
if not defined VSDIR (
    echo Visual Studio with the C++ x64 tools was not found.
    exit /b 1
)
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1

rem Ninja ships with Qt's tools and Visual Studio's CMake component
where ninja >nul 2>&1 || set "PATH=%PATH%;%QT_ROOT%\..\..\Tools\Ninja"

git submodule update --init vcpkg || exit /b 1
if not exist vcpkg\vcpkg.exe call vcpkg\bootstrap-vcpkg.bat -disableMetrics || exit /b 1

set "PATH=%QT_ROOT%\bin;%PATH%"
cmake --preset Release -G Ninja -DCLI=OFF -DCMAKE_PREFIX_PATH="%QT_ROOT%" || exit /b 1
cmake --build .build || exit /b 1

if not exist .dist\app mkdir .dist\app
copy /Y ".build\Makou Reactor.exe" .dist\app\ >nul || exit /b 1
copy /Y .build\*.dll .dist\app\ >nul
copy /Y .build\*.qm .dist\app\ >nul
windeployqt --release --no-translations --compiler-runtime ".dist\app\Makou Reactor.exe" || exit /b 1

echo.
echo Done. Run run.bat
