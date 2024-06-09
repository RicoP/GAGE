#pragma once

#include "imgui.h"
#include "raylib.h"

void scene(const char * type, const char * name) {
    TraceLog(LOG_INFO, "Scene %s %s", type, name);
}

void show(const char * character, const char * mood) {
    TraceLog(LOG_INFO, "show character %s in mood %s", character, mood);
}

void say(const char * character, const char * text) {
    TraceLog(LOG_INFO, "character %s says: %s", character, text);
}

bool choice(const char * text) {
    return ImGui::Button(text);
}

// Coroutines
        
#define YIELD(...) sm_line = __LINE__; return; case __LINE__: if(!next) return; 

#define SCENE YIELD() scene
#define SHOW YIELD() show
#define SAY YIELD() say
#define CHOICE_BEGIN(...) sm_line = __LINE__; return; case __LINE__: {};
#define CHOICE_END(...) if(!sm_did_choice) return; YIELD();
#define CHOICE(TEXT) sm_did_choice = (choice(TEXT) || sm_did_choice)
