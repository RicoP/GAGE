@echo off

setlocal EnableDelayedExpansion

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "install_path="

for /f "usebackq tokens=*" %%i in (`"!vswhere!" -latest -prerelease -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "install_path=%%i"
)

if defined install_path (
    echo Setting up compile environment...
    call "!install_path!\VC\Auxiliary\Build\vcvarsall.bat" x64
) else (
    echo Error: MSVC build tools not found.
    echo Please install Visual Studio Community
    exit /b 1
)

pushd source

DEL game.exe

CL /MD /std:c++17 /DGRAPHICS_API_OPENGL_33 /DPLATFORM_DESKTOP ^
   /DSUPPORT_FILEFORMAT_JPG ^
   /DSUPPORT_FILEFORMAT_MP3 ^
   /DGAGE_BUILD_DLL ^
   /DGAGE_BUILD_DLL_ENGINE ^
   /Iraylib/src ^
   /Iraylib/src/external ^
   /Iraylib/src/external/glfw/include ^
   /Iraylib/src/external/glfw/deps ^
   /Iimgui ^
   /Irlimgui ^
   engine.cpp ^
   gage.cpp ^
   dynamicload.cpp ^
   raylib/src/*.c ^
   imgui/*.cpp ^
   rlimgui/*.cpp ^
   /link user32.lib shell32.lib winmm.lib gdi32.lib ^
   /DLL ^
   /OUT:engine.dll

CL /MD /std:c++17 ^
   /DGAGE_BUILD_DLL ^
   /DGAGE_BUILD_DLL_GAME ^
   chapter1.cpp ^
   /link engine.lib ^
   /DLL ^
   /OUT:chapter.dll

CL /MD /std:c++17 ^
   /DGAGE_BUILD_DLL ^
   entry.cpp ^
   /link engine.lib ^
   /OUT:game.exe

game.exe

DEL *.exp *.obj *.o *.ilk *.pdb

popd