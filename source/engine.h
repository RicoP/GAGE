#pragma once

#include "raylib.h"

struct GageContext {
    Texture2D background = {0};
};

extern GageContext * s_GageContext;
