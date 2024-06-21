#include "gage.h"

#include "imgui.h"
#include "raylib.h"
#include "engine.h"
#include <stdio.h>



void scene(const char * name) {
    TraceLog(LOG_INFO, "Open Scene: %s", name);

    const char * buffer = TextFormat("../game/images/%s.jpg", name);

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

    gage_hash_t charhash = gage_hash(name);
    bool isnew = s_GageContext->activeCharacters.find(charhash) == s_GageContext->activeCharacters.end();
    Character & character = s_GageContext->activeCharacters[charhash];

    if(isnew) {
        character.fadeintimemax = 2;
        character.posx = s_GageContext->resx / 2;
        character.posy = 0;
    }

    const char * imgpath = TextFormat("../game/images/%s %s.png", name, mood);
    Texture2D newtexture = LoadTexture(imgpath);
    if(newtexture.id != 0) {
        if(character.texture.id != 0) {
            UnloadTexture(character.texture);
        }
        character.texture = newtexture;
    }
}

void say(const char * name, const char * text) {
    TraceLog(LOG_INFO, "character %s is saying: %s", name, text);

    s_GageContext->activetextcursor = 0;
    int len = std::snprintf(s_GageContext->activetext, GageContext::TEXTMAX, "%s: %s", name, text);

    if(len >= GageContext::TEXTMAX || len < 0) {
        TraceLog(LOG_FATAL, "Text is too long.");
        return;
    }
}

bool choice(const char * text) {
    return ImGui::Button(text);
}
