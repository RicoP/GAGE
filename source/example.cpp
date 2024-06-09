#include "raylib.h"
#include "rlImGui.h"	// include the API header
#include "imgui.h"

#include <stdio.h>  /* defines FILENAME_MAX */
//#define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define MAX_COLUMNS 5

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


  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );

    InitWindow(screenWidth, screenHeight, "raylib [core] example - video playback");

    Texture2D eileen = LoadTexture("../game/images/eileen happy.png");        // Texture loading

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

            ImGui::Button("Hello World");

            ClearBackground(RAYWHITE);

            DrawRectangle     ( UI(10), UI(10), UI(220), UI(80), Fade(SKYBLUE, 0.8f));
            DrawRectangleLines( UI(10), UI(10), UI(220), UI(80), BLUE);

            DrawText(TextFormat("RAYLIB VIDEO EXAMPLE (%.1f FPS)", 1.0f / GetFrameTime()), UI(20), UI(20), UI(10), BLACK);
            DrawText("- Move with keys: W, A, S, D", UI(40), UI(30), UI(10), BLACK);
            DrawText("- Mouse move to look around", UI(40), UI(50), UI(10), BLACK);
            DrawText("- Exit with Escape key", UI(40), UI(70), UI(10), BLACK);

            DrawTexture(eileen, 0, 0, WHITE);

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

/*
#ifdef _WIN32
//https://stackoverflow.com/a/39047129
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
*/