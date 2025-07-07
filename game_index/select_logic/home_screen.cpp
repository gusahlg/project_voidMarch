/*#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

#define DEG2RAD (PI / 180.0f)
#ifndef PI
    #define PI 3.14159265359f
#endif

enum GameScreen { SCREEN_MAIN, SCREEN_CREATE, SCREEN_LOAD, SCREEN_SETTINGS };

struct Star {
    float x, y, speed, size, twinkle;
};

std::vector<Star> GenerateStars(int count, int screenWidth, int screenHeight) {
    std::vector<Star> stars;
    for (int i = 0; i < count; i++) {
        stars.push_back({
            (float)GetRandomValue(0, screenWidth),
            (float)GetRandomValue(0, screenHeight),
            (float)GetRandomValue(30, 100) / 100.0f,
            (float)GetRandomValue(1, 3),
            (float)GetRandomValue(0, 360)
        });
    }
    return stars;
}

void UpdateStars(std::vector<Star>& stars, int screenWidth, int screenHeight) {
    for (auto& star : stars) {
        star.y += star.speed;
        star.twinkle += 2.0f;
        if (star.y > screenHeight) {
            star.y = 0;
            star.x = (float)GetRandomValue(0, screenWidth);
        }
    }
}

void DrawStars(const std::vector<Star>& stars) {
    for (const auto& star : stars) {
        float brightness = 0.5f + 0.5f * std::sinf(DEG2RAD * star.twinkle);
        Color color = ColorAlpha(RAYWHITE, brightness);
        DrawCircleV({ star.x, star.y }, star.size, color);
    }
}

bool DrawFancyButton(const char* text, Rectangle bounds, int fontSize, Font font, Color baseColor, Color hoverColor, Sound hoverSound, Sound clickSound, bool& hoveredLastFrame) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, bounds);

    if (hovered && !hoveredLastFrame) PlaySound(hoverSound);

    // Shadow
    DrawRectangleRounded({ bounds.x + 4, bounds.y + 4, bounds.width, bounds.height }, 0.3f, 12, Fade(BLACK, 0.3f));

    // Button
    DrawRectangleRounded(bounds, 0.3f, 12, hovered ? hoverColor : baseColor);

    // Text
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 1);
    Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 6.1f,
        bounds.y + (bounds.height - textSize.y) / 3.5f - 2 
    };
    DrawTextEx(font, text, textPos, fontSize, 1, WHITE);

    hoveredLastFrame = hovered;
    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        PlaySound(clickSound);
        return true;
    }
    return false;
}
// Figure it out.
int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED); // No title bar or borders
    InitWindow(1920, 1080, "Project Voidmarch");
    SetWindowPosition(0, 0); // Puts window at top-left
    InitAudioDevice();
    SetTargetFPS(60);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Font sciFiFont = LoadFont("resources/title_font.ttf");
    Sound hoverSound = LoadSound("resources/hover.wav");
    Sound clickSound = LoadSound("resources/click.wav");

    auto stars = GenerateStars(300, screenWidth, screenHeight);
    GameScreen currentScreen = SCREEN_MAIN;

    float btnWidth = 300, btnHeight = 60, btnSpacing = 25;
    float centerX = screenWidth / 2.0f - btnWidth / 2.0f;
    float startY = screenHeight / 2.0f - ((btnHeight + btnSpacing) * 3) / 2.0f;

    Rectangle btnCreate   = { centerX, startY, btnWidth, btnHeight };
    Rectangle btnLoad     = { centerX, startY + btnHeight + btnSpacing, btnWidth, btnHeight };
    Rectangle btnSettings = { centerX, startY + 2 * (btnHeight + btnSpacing), btnWidth, btnHeight };

    bool hoveredCreate = false, hoveredLoad = false, hoveredSettings = false;

    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        UpdateStars(stars, screenWidth, screenHeight);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawStars(stars);

        switch (currentScreen) {
            case SCREEN_MAIN:
                DrawTextEx(sciFiFont, "Project Voidmarch",
                    { screenWidth / 2.6f - MeasureTextEx(sciFiFont, "Project Voidmarch", 64, 1).x / 2, 100},
                    64, 1, WHITE);

                if (DrawFancyButton("Create Game", btnCreate, 32, sciFiFont, DARKGRAY, GRAY, hoverSound, clickSound, hoveredCreate))
                    currentScreen = SCREEN_CREATE;

                if (DrawFancyButton("Load Game", btnLoad, 32, sciFiFont, DARKGRAY, GRAY, hoverSound, clickSound, hoveredLoad))
                    currentScreen = SCREEN_LOAD;

                if (DrawFancyButton("Settings", btnSettings, 32, sciFiFont, DARKGRAY, GRAY, hoverSound, clickSound, hoveredSettings))
                    currentScreen = SCREEN_SETTINGS;
                break;

            case SCREEN_CREATE:
                DrawText("Create Game Screen", screenWidth / 2 - 120, screenHeight / 2, 30, WHITE);
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MAIN;
                break;

            case SCREEN_LOAD:
                DrawText("Load Game Screen", screenWidth / 2 - 110, screenHeight / 2, 30, WHITE);
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MAIN;
                break;

            case SCREEN_SETTINGS:
                DrawText("Settings Screen", screenWidth / 2 - 100, screenHeight / 2, 30, WHITE);
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MAIN;
                break;
        }

        EndDrawing();
    }

    UnloadFont(sciFiFont);
    UnloadSound(hoverSound);
    UnloadSound(clickSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}*/