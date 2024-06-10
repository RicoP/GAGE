#include "raylib.h"
#include "imgui.h"
#include "rlimgui.h"

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

    Texture2D background = LoadTexture("../game/images/bg washington.jpg");
    //Texture2D background = LoadTexture("../game/images/eileen happy.png");

    //SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // before your game loop
    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        // Update

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            DrawTexture(background, 0, 0, WHITE);

            // inside your game loop, between BeginDrawing() and EndDrawing()
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

            ImGui::ShowDemoWindow();
      
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