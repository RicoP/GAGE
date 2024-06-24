#pragma once

#include "raylib.h"
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

    double waittime = 0;
    bool forcetick = true;
    bool choiceactive = false;
};

extern GageContext * s_GageContext;
