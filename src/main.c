#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "raylib.h"

#include "ui.h"

#define DESIGN_WIDTH 800
#define DESIGN_HEIGHT 600

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DESIGN_WIDTH, DESIGN_HEIGHT, "Warmagic");

    SetTargetFPS(100);

    ScreenTransform t = GetScreenTransform(
        GetScreenWidth(),
        GetScreenHeight(),
        DESIGN_WIDTH,
        DESIGN_HEIGHT);

    UIElement* background = CreateSolidRect(
        (UIRect) { 0, 0, DESIGN_WIDTH, DESIGN_HEIGHT },
        WARMAGIC_STYLE_NOBORDER);

    UIElement* titleLabel = CreateLabel(
        (UIRect) { 0, 0, 260, 60 },
        "Warmagic",
        35,
        (UIAlign) { H_CENTER, V_CENTER, 0, 0, 0, 0 },
        WARMAGIC_STYLE);

    UIElement* tBackground = CreateEmptyUIElement();
    ScreenTransformUIElement(background, t, tBackground);
    UIElement* tTitleLabel = CreateEmptyUIElement();
    ScreenTransformUIElement(titleLabel, t, tTitleLabel);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            t = GetScreenTransform(
                GetScreenWidth(),
                GetScreenHeight(),
                DESIGN_WIDTH,
                DESIGN_HEIGHT);
            ScreenTransformUIElement(background, t, tBackground);
            ScreenTransformUIElement(titleLabel, t, tTitleLabel);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawUIElement(tBackground);
        DrawUIElement(tTitleLabel);
        EndDrawing();
    }

    DeleteUIElement(background);
    DeleteUIElement(tBackground);
    DeleteUIElement(titleLabel);
    DeleteUIElement(tTitleLabel);

    CloseWindow();
}