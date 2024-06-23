#!/bin/bash
pushd source

function compile_empp {
    local extra_param=$1
    em++ -DGRAPHICS_API_OPENGL_ES2 -DPLATFORM_WEB -DSUPPORT_FILEFORMAT_JPG -Wno-everything \
    -Iraylib/src \
    -Iimgui \
    -Irlimgui \
    -c $extra_param
}

function compile_emcc {
    local extra_param=$1
    emcc -DGRAPHICS_API_OPENGL_ES2 -DPLATFORM_WEB -DSUPPORT_FILEFORMAT_JPG -Wno-everything \
    -Iraylib/src \
    -Iimgui \
    -Irlimgui \
    -c $extra_param
}


compile_emcc raylib/src/raudio.c 
compile_emcc raylib/src/rcore.c 
compile_emcc raylib/src/rmodels.c 
compile_emcc raylib/src/rshapes.c 
compile_emcc raylib/src/rtext.c 
compile_emcc raylib/src/rtextures.c 
compile_emcc raylib/src/utils.c 
compile_empp chapter1.cpp 
compile_empp engine.cpp 
compile_empp gage.cpp 
compile_empp imgui/imgui.cpp 
compile_empp imgui/imgui_demo.cpp 
compile_empp imgui/imgui_draw.cpp 
compile_empp imgui/imgui_tables.cpp 
compile_empp imgui/imgui_widgets.cpp 
compile_empp rlimgui/rlimgui.cpp 

emcc raudio.o rcore.o rmodels.o rshapes.o rtext.o rtextures.o utils.o chapter1.o engine.o gage.o imgui.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o rlimgui.o -o game.html  -sUSE_GLFW=3
popd
