#pragma once

#include "imgui.h"
#include "raylib.h"

inline void scene(const char * name) {
    TraceLog(LOG_INFO, "Open Scene: %s", name);
}

inline void show(const char * name, const char * mood) {
    TraceLog(LOG_INFO, "Show character %s, mood: %s", name, mood);
}

inline void say(const char * name, const char * text) {
    TraceLog(LOG_INFO, "character %s is saying: %s", name, text);
}

inline bool choice(const char * text) {
    return ImGui::Button(text);
}

// Handmade coroutines
// https://de.wikipedia.org/wiki/Duff%E2%80%99s_Device
// https://en.wikipedia.org/wiki/Duff%27s_device
#define YIELD(...) \
    sm_line = __LINE__; \
    return; \
    case __LINE__: if(!next) return;

#define CHOICE_BEGIN(...) \
    sm_did_choice = false; \
    sm_line = __LINE__; \
    return; \
    case __LINE__: {};

#define CHOICE_END(...) if(!sm_did_choice) return; YIELD();

#define CHOICE(TEXT) sm_did_choice = choice(TEXT) || sm_did_choice

#define SCENE       YIELD(); scene
#define SHOW        YIELD(); show
#define SAY         YIELD(); say
#define RETURN(...) for(;;) { YIELD(); }
#define WAIT(TIME)  sm_wait_time = TIME;                \
                    while(sm_wait_time > 0) {           \
                        sm_wait_time -= GetFrameTime(); \
                        sm_line = __LINE__;             \
                        return;                         \
                        case __LINE__: {};              \
                    }
