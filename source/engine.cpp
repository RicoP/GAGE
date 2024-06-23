#include "raylib.h"
#include "imgui.h"
#include "rlimgui.h"
#include "chapter1.h"
#include "engine.h"
#include <cstring>

GageContext * s_GageContext = nullptr;
GageContext context;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int font_scale = 2;
    #define UI(X) (X * font_scale)

    InitWindow(screenWidth, screenHeight, "raylib [core] example - video playback");

    s_GageContext = &context;

    context.resx = screenWidth;
    context.resy = screenHeight;

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

    bool forcetick = true;

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        // Update

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            // Background
            ClearBackground(RAYWHITE);
            DrawTexture(s_GageContext->background, 0, 0, WHITE);

            bool waitForTransition = false;

            // Characters
            for(auto & [key, character] : s_GageContext->activeCharacters) {
                float t = 1;
                if(character.fadeintimemax > 0) {
                    float t0 = character.fadeintime / character.fadeintimemax;
                    t0 = gage_clamp01(t0);
                
                    character.fadeintime += GetFrameTime();
                    
                    t = character.fadeintime / character.fadeintimemax;
                    t = gage_clamp01(t);
                    if(t0 < 1 && t == 1) {
                        forcetick = true;
                    }
                }

                if(t < 1) waitForTransition = true;

                Color color = WHITE;
                int destx = character.posx - character.texture.width / 2;
                int desty = character.posy;

                int x = (int) gage_lerp(t, -character.texture.width, destx);
                int y = desty;

                color.a = (char) gage_clamp(t * 256, 0.0f, 255.0f);

                DrawTexture(character.texture, x, y, color);
            }

            // Show text
            if(*s_GageContext->activetext) {
                char textbuffer[GageContext::TEXTMAX];
                strcpy(textbuffer, s_GageContext->activetext);
                bool textscrolling = textbuffer[s_GageContext->activetextcursor] != 0;
                if(textscrolling) {
                    //static float timesincelasttext
                    waitForTransition = true;
                    s_GageContext->activetextcursor++;
                    textbuffer[s_GageContext->activetextcursor] = 0;
                }

                // Draw box for text
                {
                    int top = screenHeight - 400;
                    int bottom = 40;
                    int left = 40;
                    int right = screenWidth - 40;
                    DrawRectangle      ( 40, screenHeight - 200, screenWidth - 80, 160, Fade(SKYBLUE, 0.8f));
                    DrawRectangleLines ( 40, screenHeight - 200, screenWidth - 80, 160, BLUE);
                    DrawText(textbuffer, 40 + 16, screenHeight - 200 + 16, UI(10), BLACK);
                }
            }

            if(s_GageContext->waittime > 0) {
                waitForTransition = true;
                s_GageContext->waittime -= GetFrameTime();
            }

            // inside your game loop, between BeginDrawing() and EndDrawing()
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

            ImGui::ShowDemoWindow();

            if(!waitForTransition) {
                if(forcetick || s_GageContext->choice_active || ImGui::Button("Next Step in Script")) {
                    Chapter1();
                    forcetick = false;
                }
            }

            rlImGuiEnd();			// ends the ImGui content mode. Make all ImGui calls before this
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    rlImGuiShutdown();		// cleans up ImGui

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}