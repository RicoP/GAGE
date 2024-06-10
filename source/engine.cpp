#include "raylib.h"
#include "imgui.h"
#include "rlimgui.h"
#include <cstdio>

#define MAX_COLUMNS 5

bool my_tool_active = true;

inline void exampleWindow() 
{
    // Create a window called "My First Tool", with a menu bar.
    ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    /*
    // Edit a color stored as 4 floats
    ImGui::ColorEdit4("Color", my_color);

    // Generate samples and plot them
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++)
        ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
    */
    ImGui::End();
}

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

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera3D camera = { 0 };
    camera.position = Vector3{ 4.0f, 2.0f, 4.0f };
    camera.target = Vector3{ 0.0f, 1.8f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Generates some random columns
    float heights[MAX_COLUMNS] = { 0 };
    Vector3 positions[MAX_COLUMNS] = { 0 };
    Color colors[MAX_COLUMNS] = { 0 };

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 4);
        positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
        colors[i] = Color{ (unsigned char)GetRandomValue(20, 255), (unsigned char)GetRandomValue(10, 55), (unsigned char)30, (unsigned char)255 };
    }

    //SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // before your game loop
    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // inside your game loop, between BeginDrawing() and EndDrawing()
            rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

            exampleWindow();
            ImGui::ShowDemoWindow();
            if (ImGui::Button("Save"))
            {
                std::puts("Hello World");
            }

            BeginMode3D(camera);
            {
                DrawPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground

                // Draw some cubes around
                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
                }
            }
            EndMode3D();

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