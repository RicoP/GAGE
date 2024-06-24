#pragma once

#include "imgui.h"
#include "raylib.h"
#include <stdio.h>
#include <map>

typedef unsigned long long gage_hash_t;

constexpr gage_hash_t gage_hash(const char* pBuffer) {
    gage_hash_t  MagicPrime = 0x00000100000001b3;
    gage_hash_t  Hash       = 0xcbf29ce484222325;

    for ( ; *pBuffer; pBuffer++)
        Hash = (Hash ^ *pBuffer) * MagicPrime;   // bitweises XOR und dann Multiplikation

    return Hash;
}

struct GageCharacter {
    Texture2D texture = {0};
    int x = 0;
    int y = 0;
};

struct GageContext {
    enum {
        TEXT_MAX = 1024
    };

    Music music = {0};
    bool isMusicPlaying = false;

    Texture2D background = {0};
    std::map<gage_hash_t, GageCharacter> characters;

    char text[TEXT_MAX] = "";
    int  textcursor = 0;
};

extern GageContext * s_GageContext;

inline void scene(const char * name) {
    TraceLog(LOG_INFO, "Open Scene: %s", name);

    const char * path = TextFormat("../game/images/%s.jpg", name);

    Texture2D newtexture = LoadTexture(path);
    if(newtexture.id != 0) {
        if(s_GageContext->background.id != 0) {
            UnloadTexture(s_GageContext->background);
        }
    
        s_GageContext->background = newtexture;
    }
    else {
        TraceLog(LOG_FATAL, "scene couldn't load texture '%s'.", path);
    }
}

inline void show(const char * name, const char * mood) {
    TraceLog(LOG_INFO, "Show character %s, mood: %s", name, mood);

    gage_hash_t namehash = gage_hash(name);

    bool isnew = s_GageContext->characters.find(namehash) == s_GageContext->characters.end();
    GageCharacter & character = s_GageContext->characters[namehash];

    const char * path = TextFormat("../game/images/%s %s.png", name, mood);

    Texture2D newtexture = LoadTexture(path);
    if(newtexture.id != 0) {
        if(character.texture.id != 0) {
            UnloadTexture(character.texture);
        }
    
        character.texture = newtexture;
    }
    else {
        TraceLog(LOG_FATAL, "show couldn't load texture '%s'.", path);
    }

    if(isnew) {
        character.x = GetScreenWidth() / 2 - character.texture.width / 2;
        character.y = 0;
    }
}

inline void say(const char * name, const char * text) {
    TraceLog(LOG_INFO, "character %s is saying: %s", name, text);

    s_GageContext->textcursor = 0;
    int len = std::snprintf(s_GageContext->text, GageContext::TEXT_MAX, "%s: %s", name, text);
    if(len >= GageContext::TEXT_MAX || len < 0) {
        TraceLog(LOG_FATAL, "Text is too long.");
        return;
    }
}

inline bool choice(const char * text) {
    return ImGui::Button(text);
}

inline void music(const char * title) {
    TraceLog(LOG_INFO, "Play music track %s", title);

    const char * path = TextFormat("../game/music/%s.mp3", title);

    if(s_GageContext->isMusicPlaying) {
        StopMusicStream(s_GageContext->music);
        UnloadMusicStream(s_GageContext->music);
    }
    
    s_GageContext->music = LoadMusicStream(path);
    PlayMusicStream(s_GageContext->music);
    s_GageContext->isMusicPlaying = true;
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

#define SCENE       scene
#define MUSIC       music
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
