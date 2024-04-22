#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif
#undef near
#undef far

#include "raylib.h"
#include "Jolt/Jolt.h"
#include "box2d/box2d.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

int main(int argc, char **argv)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    rlImGuiSetup(true);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            
            //imgui     
            {
            rlImGuiBegin();	
            ImGui::Text("This is some useful text.");
            
            rlImGuiEnd();
            }	
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    rlImGuiShutdown();	
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}