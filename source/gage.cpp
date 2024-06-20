#include "gage.h"

#include "imgui.h"
#include "raylib.h"
#include "engine.h"
#include <stdio.h>

void scene(const char * name) {
    TraceLog(LOG_INFO, "Open Scene: %s", name);

    const int BUFFER_SIZE = 1024; 

    char buffer[BUFFER_SIZE];
    int size = snprintf(buffer, BUFFER_SIZE, "../game/images/%s.jpg", name);

    if(size >= BUFFER_SIZE || size < 0) {
        TraceLog(LOG_FATAL, "scene parameter is too big.");
        return;
    }

    Texture2D newtexture = LoadTexture(buffer);
    if(newtexture.id != 0) {
        if(s_GageContext->background.id != 0) {
            UnloadTexture(s_GageContext->background);
        }
    
        s_GageContext->background = newtexture;
    }
    else {
        TraceLog(LOG_FATAL, "scene couldn't load texture '%s'.", buffer);
    }
}

void show(const char * name, const char * mood) {
    TraceLog(LOG_INFO, "Show character %s, mood: %s", name, mood);
}

void say(const char * name, const char * text) {
    TraceLog(LOG_INFO, "character %s is saying: %s", name, text);
}

bool choice(const char * text) {
    return ImGui::Button(text);
}
