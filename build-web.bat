@echo off

pushd source

em++ -DGRAPHICS_API_OPENGL_ES2 -DPLATFORM_WEB -DSUPPORT_FILEFORMAT_JPG -Wno-everything ^
    -Iraylib/src ^
    -Iraylib/src/external ^
    -Iraylib/src/external/glfw/include ^
    -Iraylib/src/external/glfw/deps ^
    -Iimgui ^
    -Irlimgui ^
    -x c ^
    raylib/src/raudio.c ^
    raylib/src/rcore.c ^
    raylib/src/rglfw.c ^
    raylib/src/rmodels.c ^
    raylib/src/rshapes.c ^
    raylib/src/rtext.c ^
    raylib/src/rtextures.c ^
    raylib/src/utils.c ^
    chapter1.cpp ^
    engine.cpp ^
    gage.cpp ^
    imgui/imgui.cpp ^
    imgui/imgui_demo.cpp ^
    imgui/imgui_draw.cpp ^
    imgui/imgui_tables.cpp ^
    imgui/imgui_widgets.cpp ^
    rlimgui/rlimgui.cpp ^
    -o hello.html
    
popd