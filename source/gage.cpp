#include "gage.h"
#include "engine.h"
#include "imgui.h"
#include <stdio.h>

void scene(const char * name) {
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

void show(const char * name, const char * mood) {
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

void say(const char * name, const char * text) {
    TraceLog(LOG_INFO, "character %s is saying: %s", name, text);

    s_GageContext->textcursor = 0;
    int len = std::snprintf(s_GageContext->text, GageContext::TEXT_MAX, "%s: %s", name, text);
    if(len >= GageContext::TEXT_MAX || len < 0) {
        TraceLog(LOG_FATAL, "Text is too long.");
        return;
    }
}
void choice_begin() {
    s_GageContext->choiceactive = true;
}

bool choice(const char * text) {
    return ImGui::Button(text);
}

void choice_end() {
    s_GageContext->choiceactive = false;
}

void music(const char * title) {
    return;

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

void wait(double seconds) {
    s_GageContext->waittime = seconds;
}