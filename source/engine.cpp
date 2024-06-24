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

    InitWindow(screenWidth, screenHeight, "GAGE");

    InitAudioDevice();              // Initialize audio device

    s_GageContext = &context;

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        bool waitfortransition = false;

        // Update
        if(s_GageContext->isMusicPlaying) {
            UpdateMusicStream(s_GageContext->music);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            DrawTexture(s_GageContext->background, 0, 0, WHITE);

            for(auto & [key, character] : s_GageContext->characters) {
                DrawTexture(character.texture, character.x, character.y, WHITE);
            }

            // inside your game loop, between BeginDrawing() and EndDrawing()
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

            ImGui::ShowDemoWindow();

            // Draw Textbox
            {
                if(s_GageContext->text[0]) {
                    char buffer[GageContext::TEXT_MAX];
                    std::strcpy(buffer, s_GageContext->text);
                    bool textscrolling = buffer[s_GageContext->textcursor] != 0;
                    if(textscrolling) {
                        waitfortransition = true;
                        s_GageContext->textcursor++;
                        buffer[s_GageContext->textcursor] = 0;
                    }

                    DrawRectangle      ( 40, GetScreenHeight() - 200, GetScreenWidth() - 80, 160, Fade(SKYBLUE, 0.8f));
                    DrawRectangleLines ( 40, GetScreenHeight() - 200, GetScreenWidth() - 80, 160, BLUE);
                    DrawText(buffer, 40 + 16, GetScreenHeight() - 200 + 16, UI(10), BLACK);
                }
            }

            // check waittime
            if(s_GageContext->waittime > 0) {
                s_GageContext->waittime -= GetFrameTime();
                if(s_GageContext->waittime <= 0) {
                    s_GageContext->forcetick = true;
                }
                else {
                    waitfortransition = true;
                }
            }

            if(!waitfortransition) {
                if(s_GageContext->choiceactive) s_GageContext->forcetick = true;

                if(s_GageContext->forcetick || ImGui::Button("Next Step in Script")) {
                    Chapter1();
                    s_GageContext->forcetick = false;
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