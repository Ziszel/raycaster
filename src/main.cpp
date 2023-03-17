#include <raylib-cpp.hpp>
#include <memory>
#include "Player.h"
#include "Map.h"

int main() {
    
    // Initialization
    const int screen_width = 1280;
    const int screen_height = 720;
    float delta_time = 60.0f;

    raylib::Color textColor(RAYWHITE);
    raylib::Window w(screen_width, screen_height, "2D Raycasting example");
    
    SetTargetFPS(60);

    // set up the game objects
    std::unique_ptr<Player> p = std::make_unique<Player>(
            raylib::Vector2(screen_width / 4, screen_height / 2),
            raylib::Vector2(32, 32),
            0.0f);

    std::unique_ptr<Map> m = std::make_unique<Map>();

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Update
        delta_time = GetFrameTime();
        p->Update(delta_time, m);

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        m->Draw();
        p->Draw(delta_time);
        EndDrawing();
    }

    return 0;
}