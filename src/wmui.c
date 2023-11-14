#include "wm.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "raylib.h"

//#include <stdio.h>

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

//const char* fontJupiterCFile = "res/jupiterc.ttf";

// -----------------------------------------------------------------------------
// Type Utilities
// -----------------------------------------------------------------------------

Vector2 Vector2FromUIPoint(UIPoint point)
{
    return (Vector2) { point.x, point.y };
}

Rectangle RectangleFromUIRect(UIRect rect)
{
    return (Rectangle)
    {
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top
    };
}

UIPoint GetTopLeftUIRect(UIRect rect)
{
    return (UIPoint) { rect.left, rect.top };
}

// -----------------------------------------------------------------------------
// Screen Transforms
// -----------------------------------------------------------------------------

ScreenTransform GetScreenTransform(int w, int h, int ow, int oh)
{
    float scale = min((float)w / ow, (float)h / oh);
    float xstart = ((float)w - ((float)ow * scale)) * 0.5f;
    float ystart = ((float)h - ((float)oh * scale)) * 0.5f;
    return (ScreenTransform) { xstart, ystart, scale };
}

UIPoint ScreenTransformUIPoint(UIPoint point, ScreenTransform transform)
{
    return (UIPoint)
    {
        point.x * transform.scale + transform.xstart,
        point.y * transform.scale + transform.ystart
    };
}

UIRect ScreenTransformUIRect(UIRect rect, ScreenTransform transform)
{
    return (UIRect)
    {
        rect.left + transform.xstart,
        rect.top + transform.ystart,
        rect.right * transform.scale + transform.xstart,
        rect.bottom * transform.scale + transform.ystart
    };
}

UIText ScreenTransformUIText(UIText text, ScreenTransform transform)
{
    return (UIText)
    {
        text.str,
        // idk why but I have to do this powf adjustment to make the text scale correctly??
        text.fontSize * powf(transform.scale, 1.2f),
        text.fontColor
    };
}

// -----------------------------------------------------------------------------
// UI Collisions
// -----------------------------------------------------------------------------

bool CheckCollisionUIRectUIPoint(UIRect rect, UIPoint point)
{
    return
        point.x > rect.left
        && point.y > rect.top
        && point.x < rect.right
        && point.y < rect.bottom;
}

// -----------------------------------------------------------------------------
// Basic UI Drawing
// -----------------------------------------------------------------------------

void DrawUIRect(UIRect rect, Color color)
{
    DrawRectangleRec(RectangleFromUIRect(rect), color);
}

void DrawUIRectTransform(UIRect rect, Color color, ScreenTransform transform)
{
    rect = ScreenTransformUIRect(rect, transform);
    DrawRectangleRec(RectangleFromUIRect(rect), color);
}

void DrawUIBorder(UIRect rect, float width, Color color)
{
    DrawRectangleRoundedLines(RectangleFromUIRect(rect), 0.1f, 0, width, color);
}

void DrawUIBorderTransform(UIRect rect, float width, Color color, ScreenTransform transform)
{
    rect = ScreenTransformUIRect(rect, transform);
    DrawRectangleRoundedLines(RectangleFromUIRect(rect), 0.1f, 0, width, color);
}

void DrawUIText(UIPoint loc, UIText text)
{
    float spacing = 0.1f * text.fontSize;

    int strsize = strlen(text.str) + 1;
    char str[strsize];
    memcpy(str, text.str, strsize);
    
    char* line = NULL;
    char* rest = NULL;

    int n = 0;
    for (line = __strtok_r(str, "\n", &rest); line != NULL; line = __strtok_r(NULL, "\n", &rest))
    {
        Vector2 linepos = { loc.x, loc.y + text.fontSize * n + spacing * n };
        DrawTextEx(GetFontDefault(), line, linepos, text.fontSize, spacing, text.fontColor);
        ++n;
    }
}

void DrawUITextTransform(UIPoint loc, UIText text, ScreenTransform transform)
{
    loc = ScreenTransformUIPoint(loc, transform);
    text = ScreenTransformUIText(text, transform);

    float spacing = 0.1f * text.fontSize;

    int strsize = strlen(text.str) + 1;
    char str[strsize];
    memcpy(str, text.str, strsize);
    
    char* line = NULL;
    char* rest = NULL;

    int n = 0;
    for (line = __strtok_r(str, "\n", &rest); line != NULL; line = __strtok_r(NULL, "\n", &rest))
    {
        Vector2 linepos = { loc.x, loc.y + text.fontSize * n + spacing * n };
        DrawTextEx(GetFontDefault(), line, linepos, text.fontSize, spacing, text.fontColor);
        ++n;
    }
}

// -----------------------------------------------------------------------------
// UI Controls
// -----------------------------------------------------------------------------

UIBase CreateUIBase(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor)
{
    return (UIBase) { rect, backgroundColor, borderWidth, borderColor };
}

Label CreateLabel(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor, UIText text)
{
    UIBase base = CreateUIBase(rect, backgroundColor, borderWidth, borderColor);
    return (Label) { base, text };
}

Button CreateButton(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor, UIText text)
{
    UIBase base = CreateUIBase(rect, backgroundColor, borderWidth, borderColor);
    return (Button) { base, text };
}

ToggleButton CreateToggleButton(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor, UIText text, bool isToggled)
{
    UIBase base = CreateUIBase(rect, backgroundColor, borderWidth, borderColor);
    return (ToggleButton) { base, text, isToggled };
}

void DrawUIBase(UIBase base)
{
    DrawUIRect(base.rect, base.backgroundColor);
    DrawUIBorder(base.rect, base.borderWidth, base.borderColor);
}

void DrawUIBaseTransform(UIBase base, ScreenTransform transform)
{
    base.rect = ScreenTransformUIRect(base.rect, transform);
    DrawUIRect(base.rect, base.backgroundColor);
    DrawUIBorder(base.rect, base.borderWidth, base.borderColor);
}

void DrawLabel(Label label)
{
    DrawUIBase(label.base);
    DrawUIText(GetTopLeftUIRect(label.base.rect), label.text);
}

void DrawLabelTransform(Label label, ScreenTransform transform)
{
    label.base.rect = ScreenTransformUIRect(label.base.rect, transform);
    label.text = ScreenTransformUIText(label.text, transform);
    DrawUIBase(label.base);
    DrawUIText(GetTopLeftUIRect(label.base.rect), label.text);
}

void DrawButton(Button button)
{
    DrawUIBase(button.base);
    DrawUIText(GetTopLeftUIRect(button.base.rect), button.text);
}

void DrawButtonTransform(Button button, ScreenTransform transform)
{
    button.base.rect = ScreenTransformUIRect(button.base.rect, transform);
    button.text = ScreenTransformUIText(button.text, transform);
    DrawUIBase(button.base);
    DrawUIText(GetTopLeftUIRect(button.base.rect), button.text);
}

void DrawToggleButton(ToggleButton toggleButton)
{
    DrawUIBase(toggleButton.base);
    DrawUIText(GetTopLeftUIRect(toggleButton.base.rect), toggleButton.text);
}

void DrawToggleButtonTransform(ToggleButton toggleButton, ScreenTransform transform)
{
    toggleButton.base.rect = ScreenTransformUIRect(toggleButton.base.rect, transform);
    toggleButton.text = ScreenTransformUIText(toggleButton.text, transform);
    DrawUIBase(toggleButton.base);
    DrawUIText(GetTopLeftUIRect(toggleButton.base.rect), toggleButton.text);
}

UIElement CreateEmptyUIElement(UIBase base)
{
    return (UIElement) { base, NONE, NULL };
}

UIElement CreateUIElement(UIElementType type, void* child)
{
    switch (type)
    {
        case LABEL:
        Label* pLabel = (Label*)child;
        return (UIElement) { pLabel->base, LABEL, pLabel };

        case BUTTON:
        Button* pButton = (Button*)child;
        return (UIElement) { pButton->base, BUTTON, pButton };

        case TOGGLEBUTTON:
        ToggleButton* pToggleButton = (ToggleButton*)child;
        return (UIElement) { pToggleButton->base, BUTTON, pToggleButton };

        default:
        return CreateEmptyUIElement(ZERO_UIBASE);
    }
}