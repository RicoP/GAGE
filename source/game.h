#pragma once

#include "imgui.h"

void scene(const char * type, const char * name) {
    ImGui::Text("Scene %s %s", type, name);
}

void show(const char * character, const char * mood) {
    ImGui::Text("show character %s in mood %s", character, mood);
}

void say(const char * character, const char * text) {
    ImGui::Text("character %s says: %s", character, text);
}

bool choice(const char * text) {
    return ImGui::CollapsingHeader(text);
}

#define SCENE scene
#define SHOW show
#define SAY say
#define CHOICE choice
#define CHOICE_YIELD(...)
