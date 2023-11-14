#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "raylib.h"

#include "wm.h"

#define DESIGN_WIDTH 800
#define DESIGN_HEIGHT 600

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DESIGN_WIDTH, DESIGN_HEIGHT, "Warmagic");

    ScreenTransform transform = GetScreenTransform(
        GetScreenWidth(),
        GetScreenHeight(),
        DESIGN_WIDTH,
        DESIGN_HEIGHT);

    UIRect background = { 0, 0, 800, 600 };

    Label titleLabel = CreateLabel(
        (UIRect) { 10, 10, 260, 50 },
        BLACK,
        3,
        DARKPURPLE,
        (UIText) { "Warmagic", 35, PURPLE });

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            transform = GetScreenTransform(
                GetScreenWidth(),
                GetScreenHeight(),
                DESIGN_WIDTH,
                DESIGN_HEIGHT);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawUIRectTransform(background, BLACK, transform);
        DrawLabelTransform(titleLabel, transform);
        EndDrawing();
    }

    CloseWindow();
}