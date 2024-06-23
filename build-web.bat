@echo off

pushd source

DEL *.exe

clang++ -DGRAPHICS_API_OPENGL_33 -DPLATFORM_DESKTOP -DSUPPORT_FILEFORMAT_JPG -Wno-everything ^
    -Iraylib/src ^
    -Iraylib/src/external ^
    -Iraylib/src/external/glfw/include ^
    -Iraylib/src/external/glfw/deps ^
    -Iimgui ^
    -Irlimgui ^
    chapter1.cpp ^
    engine.cpp ^
    gage.cpp ^
    imgui/*.cpp ^
    rlimgui/*.cpp ^
    -x c ^
    raylib/src/*.c ^
    -luser32 -lshell32 -lwinmm -lgdi32 ^
    -o game.exe

popd