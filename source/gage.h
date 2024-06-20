#pragma once

#include "raylib.h"

void scene(const char * name);
void show(const char * name, const char * mood);
void say(const char * name, const char * text);
bool choice(const char * text);

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

#define SCENE       scene
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
