@echo off
rem Runs the build in .dist\app (made by build.bat).
if not exist ".dist\app\Makou Reactor.exe" (
    echo Not built yet. Run build.bat first.
    exit /b 1
)
start "" /D ".dist\app" ".dist\app\Makou Reactor.exe" %*
