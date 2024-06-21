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

#define CHOICE_END(...) do { if(!sm_did_choice) return; YIELD(); } while(0)
#define CHOICE(TEXT)    sm_did_choice = choice(TEXT) || sm_did_choice
#define SCENE(...)      do { scene(__VA_ARGS__); YIELD(); } while(0)
#define SHOW(...)       do { show(__VA_ARGS__);  YIELD(); } while(0)
#define SAY(...)        do { say(__VA_ARGS__);   YIELD(); } while(0)
#define RETURN(...)     do { for(;;) { YIELD(); } } while(0)
#define WAIT(TIME)      do {                                    \
                            sm_wait_time = TIME;                \
                            while(sm_wait_time > 0) {           \
                                sm_wait_time -= GetFrameTime(); \
                                sm_line = __LINE__;             \
                                return;                         \
                                case __LINE__: {};              \
                            }                                   \
                        } while(0)
