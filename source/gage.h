#pragma once

#ifdef GAGE_BUILD_DLL
#   if defined(GAGE_BUILD_DLL_ENGINE)
#       define GAGE_API __declspec(dllexport)
#   elif defined(GAGE_BUILD_DLL_GAME)
#       define GAGE_API __declspec(dllimport)
#   else
#       error "UNKNOWN DLL type"
#   endif
#else
#define GAGE_API
#endif

GAGE_API void scene(const char * name);
GAGE_API void show(const char * name, const char * mood);
GAGE_API void say(const char * name, const char * text);
GAGE_API void choice_begin();
GAGE_API bool choice(const char * text);
GAGE_API void choice_end();
GAGE_API void music(const char * title);
GAGE_API void wait(double seconds);

// Handmade coroutines
// https://de.wikipedia.org/wiki/Duff%E2%80%99s_Device
// https://en.wikipedia.org/wiki/Duff%27s_device
#define YIELD(...) \
    sm_line = __LINE__; \
    return; \
    case __LINE__: {};

#define CHOICE_BEGIN(...) \
    choice_begin();        \
    sm_did_choice = false; \
    sm_line = __LINE__; \
    return; \
    case __LINE__: {};

#define CHOICE(TEXT) sm_did_choice = choice(TEXT) || sm_did_choice

#define CHOICE_END(...) do { if(!sm_did_choice) return; choice_end(); } while(0)
#define SCENE(...)      do { scene(__VA_ARGS__); } while(0)
#define MUSIC(...)      do { music(__VA_ARGS__); } while(0)
#define SHOW(...)       do { show(__VA_ARGS__); YIELD(); } while(0)
#define SAY(...)        do { say(__VA_ARGS__); YIELD(); } while(0)
#define WAIT(...)       do { wait(__VA_ARGS__); YIELD(); } while(0);
#define RETURN(...)     do { for(;;) { YIELD(); } } while(0)
