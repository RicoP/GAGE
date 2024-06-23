#pragma once

#include "raylib.h"
#include <map>

typedef unsigned long long gage_hash_t;

// https://de.wikipedia.org/wiki/FNV_(Informatik)
constexpr gage_hash_t gage_hash (const char* pBuffer)
{
    gage_hash_t MagicPrime = 0x00000100000001b3ULL;
    gage_hash_t Hash       = 0xcbf29ce484222325ULL;

    for ( ; *pBuffer; pBuffer++)
        Hash = (Hash ^ *pBuffer) * MagicPrime;   // bitweises XOR und dann Multiplikation

    return Hash;
}

struct Character {
    Texture2D texture = {0};
    int posx = 0;
    int posy = 0;
    float fadeintime = 0;
    float fadeintimemax = 0;
};

struct GageContext {
    enum {
        TEXTMAX = 1024
    };
    int resx = 0;
    int resy = 0;
    Texture2D background = {0};
    std::map<gage_hash_t, Character> activeCharacters;
    char activetext[TEXTMAX] = "";
    int activetextcursor = 0;
    double waittime = 0;
    bool choice_active = false;
};

extern GageContext * s_GageContext;

// math
inline float gage_lerp(float t, float min, float max) {
    return min + t * (max - min);
}

inline float gage_clamp(int value, int min, int max) {
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

inline float gage_clamp(float value, float min, float max) {
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

inline float gage_clamp(double value, double min, double max) {
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

template<class T>
inline T gage_clamp01(T value) {
    if(value < 0) return 0;
    if(value > 1) return 1;
    return value;
}