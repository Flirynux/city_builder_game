/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"   // NOTE: Declares global (extern) variables and screens functions
#include "traffic.cpp"
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
Font font = {};
Music music = {};
Sound fxCoin = {};

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;

std::weak_ptr<trafficNode> selectedA;
std::weak_ptr<trafficNode> selectedB;

float timer = 1;
RoadManager roadmanager;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    roadmanager.createNode(50,50);
    roadmanager.createNode(150,150);
    roadmanager.createNode(50,150);
    roadmanager.createNode(150,50);

    roadmanager.createNode(350,250);
    roadmanager.createNode(250,350);
    roadmanager.createNode(350,350);




    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");

    InitAudioDevice();      // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    /*
    //music = LoadMusicStream("resources/ambient.ogg"); // TODO: Load music
    fxCoin = LoadSound("resources/coin.wav");

    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);
    */
    // Setup and init first screen

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // Unload global data loaded
    UnloadFont(font);
    /*
    UnloadMusicStream(music);
    UnloadSound(fxCoin);
    */
    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


// Update and draw game frame
static void UpdateDrawFrame(void)
{
    timer -= GetFrameTime();
    if(timer < 0){
        timer = 1;
        roadmanager.updateTraffic();
    }

    
    if(IsKeyPressed(KEY_DELETE)){
        if (auto nodeToKill = selectedA.lock()) {
        roadmanager.removeNode(nodeToKill);
        // No need to manually nullify selectedA; 
        // next frame .lock() will simply fail!
        }
    }
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // We need a way to check which node is hovered from the manager
        auto clickedNode = roadmanager.getHoveredNode(); 
        if(IsKeyDown(KEY_LEFT_CONTROL)){
            if (clickedNode) {
                selectedB = clickedNode;
            } else
            {
                selectedB.reset();
            }
        }else{
            if (clickedNode) {
                selectedA = clickedNode;
            } else
            {
                selectedA.reset();
            }
        }
        
    }
    
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        if (auto node = selectedA.lock()) {
            Vector2 mouse = GetMousePosition();
            node->x = mouse.x;
            node->y = mouse.y;
        }
        
    }

    if(IsKeyPressed(KEY_F)){
        if (auto nodeA = selectedA.lock()) {
            if(auto nodeB = selectedB.lock()){
                roadmanager.connectTwoWay(nodeA,nodeB);
            }
        }
    }
    if(IsKeyPressed(KEY_G)){
        if (auto nodeA = selectedA.lock()) {
            if(auto nodeB = selectedB.lock()){
                roadmanager.connectOneWay(nodeA,nodeB);
            }
        }
    }
    if(IsKeyPressed(KEY_A)){
        if (auto node = selectedA.lock()) {
            node->trafficAmount += 50;
        }
    }
    if(IsKeyPressed(KEY_D)){
        auto pos = GetMousePosition();
        roadmanager.createNode(pos.x,pos.y);
    }
    
    // Update
    //----------------------------------------------------------------------------------
    //UpdateMusicStream(music);       // NOTE: Music keeps playing between screens
    
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    
        ClearBackground(BLACK);
        roadmanager.drawAll();

        if (auto sel = selectedA.lock()) {
            DrawCircleLines(sel->x, sel->y, 10+sel->trafficAmount/10, SKYBLUE); 
            DrawText("SELECTED", sel->x - 20, sel->y - 30, 32, SKYBLUE);
        }
        if (auto sel = selectedB.lock()) {
            DrawCircleLines(sel->x, sel->y, 10+sel->trafficAmount/10, RED); 
            DrawText("SELECTED", sel->x - 20, sel->y - 30, 32, RED);
        }
    
        DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
