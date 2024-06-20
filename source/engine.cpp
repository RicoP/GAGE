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
                character.fadeintime += GetFrameTime();
                float t = 1;
                if(character.fadeintimemax > 0) {
                    t = character.fadeintime / character.fadeintimemax;
                    t = gage_clamp01(t);
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

            // inside your game loop, between BeginDrawing() and EndDrawing()
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

            ImGui::ShowDemoWindow();

            if(!waitForTransition) {
                Chapter1( ImGui::Button("Next Step in Script") );
            }
      
            DrawRectangle     ( UI(10), UI(10), UI(220), UI(80), Fade(SKYBLUE, 0.8f));
            DrawRectangleLines( UI(10), UI(10), UI(220), UI(80), BLUE);

            DrawText(TextFormat("RAYLIB VIDEO EXAMPLE (%.1f FPS)", 1.0f / GetFrameTime()), UI(20), UI(20), UI(10), BLACK);
            DrawText("- Move with keys: W, A, S, D", UI(40), UI(30), UI(10), BLACK);
            DrawText("- Mouse move to look around", UI(40), UI(50), UI(10), BLACK);
            DrawText("- Exit with Escape key", UI(40), UI(70), UI(10), BLACK);

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