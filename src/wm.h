#ifndef WM_H
#define WM_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "raylib.h"

#define max(a, b)            \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a, b)            \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

// -----------------------------------------------------------------------------
// Module: wmui.c
// -----------------------------------------------------------------------------

typedef struct ScreenTransform
{
    float xstart;
    float ystart;
    float scale;
} ScreenTransform;

typedef struct UIPoint
{
    float x;
    float y;
} UIPoint;

typedef struct UIRect
{
    float left;
    float top;
    float right;
    float bottom;
} UIRect;

typedef struct UIText
{
    char* str;
    float fontSize;
    Color fontColor;
} UIText;

typedef struct UIBase
{
    UIRect rect;
    Color backgroundColor;
    float borderWidth;
    Color borderColor;
} UIBase;

typedef struct Label
{
    UIBase base;
    UIText text;
} Label;

typedef struct Button
{
    UIBase base;
    UIText text;
} Button;

typedef struct ToggleButton
{
    UIBase base;
    UIText text;
    bool isToggled;
} ToggleButton;

typedef enum UIElementType
{
    NONE, LABEL, BUTTON, TOGGLEBUTTON
} UIElementType;

typedef struct UIElement
{
    UIBase base;
    UIElementType type;
    void* child;
} UIElement;

#define ZERO_UIRECT (UIRect) { 0, 0, 0, 0 }
#define ZERO_UIBASE (UIBase) { ZERO_UIRECT, BLANK, 0, BLANK }

static inline int GetFontSizeFromPixels(int pixels) { return (int)(pixels * 0.75f); }
static inline int GetPixelsFromFontSize(int fontSize) { return (int)(fontSize * 1.33f); }

Vector2 Vector2FromUIPoint(UIPoint point);
Rectangle RectangleFromUIRect(UIRect rect);
UIPoint GetTopLeftUIRect(UIRect rect);

ScreenTransform GetScreenTransform(int w, int h, int ow, int oh);
UIPoint ScreenTransformUIPoint(UIPoint point, ScreenTransform transform);
UIRect ScreenTransformUIRect(UIRect rect, ScreenTransform transform);
UIText ScreenTransformUIText(UIText text, ScreenTransform transform);

bool CheckCollisionUIRectUIPoint(UIRect rect, UIPoint point);

void DrawUIRect(UIRect rect, Color color);
void DrawUIRectTransform(UIRect rect, Color color, ScreenTransform transform);
void DrawUIBorder(UIRect rect, float width, Color color);
void DrawUIBorderTransform(UIRect rect, float width, Color color, ScreenTransform transform);
void DrawUIText(UIPoint loc, UIText text);
void DrawUITextTransform(UIPoint loc, UIText text, ScreenTransform transform);

UIBase CreateUIBase(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor);
Label CreateLabel(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor, UIText text);
Button CreateButton(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor, UIText text);
ToggleButton CreateToggleButton(UIRect rect, Color backgroundColor, float borderWidth, Color borderColor, UIText text, bool isToggled);
void DrawUIBase(UIBase base);
void DrawUIBaseTransform(UIBase base, ScreenTransform transform);
void DrawLabel(Label label);
void DrawLabelTransform(Label label, ScreenTransform transform);
void DrawButton(Button button);
void DrawToggleButton(ToggleButton toggleButton);

UIElement CreateEmptyUIElement(UIBase base);
UIElement CreateUIElement(UIElementType type, void* child);

// -----------------------------------------------------------------------------
// Module: wmtraveler.c
// -----------------------------------------------------------------------------

// definitions here

// -----------------------------------------------------------------------------
// Module: wmparty.c
// -----------------------------------------------------------------------------

// definitions here

// -----------------------------------------------------------------------------
// Module: wmgear.c
// -----------------------------------------------------------------------------

// definitions here

#endif