#include "ui.h"

#include <stdbool.h>
#include <stddef.h>
#include <alloca.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#define FONT_SIZE_SPACING_FACTOR 0.1f

// -----------------------------------------------------------------------------

Vector2 UIPointToVector2(UIPoint point)
{
    return (Vector2) { point.x, point.y };
}

Rectangle UIRectToRectangle(UIRect rect)
{
    return (Rectangle)
    {
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top
    };
}

// -----------------------------------------------------------------------------

UISize GetUIRectSize(UIRect rect)
{
    return (UISize) { rect.right - rect.left, rect.bottom - rect.top };
}

UISize GetUITextSize(UIText text)
{
    float spacing = text.fontSize * FONT_SIZE_SPACING_FACTOR;
    UISize totalSize = { 0, 0 };
    size_t strSize = strlen(text.str) + 1;
    char* str = alloca(strSize);
    strncpy(str, text.str, strSize);

    char* token = NULL;
    for (token = strtok(str, "\n"); token != NULL; token = strtok(NULL, "\n"))
    {
        Vector2 lineSize = MeasureTextEx(GetFontDefault(), token, text.fontSize, spacing);
        totalSize.w = max(totalSize.w, lineSize.x);
        totalSize.h += lineSize.y + spacing;
    }
    totalSize.h -= spacing;

    return totalSize;
}

bool CollidesUIRectUIPoint(UIRect rect, UIPoint point)
{
    return
           point.x > rect.left  && point.y > rect.top
        && point.x < rect.right && point.y < rect.bottom;
}

UIPoint GetCenterUIRect(UIRect rect)
{
    return (UIPoint)
    {
        (rect.left + rect.right) * 0.5f,
        (rect.top + rect.bottom) * 0.5f
    };
}

UIRect SetCenterUIRect(UIRect rect, UIPoint center)
{
    float halfw = (rect.right - rect.left) * 0.5f;
    float halfh = (rect.bottom - rect.top) * 0.5f;
    return (UIRect)
    {
        center.x - halfw,
        center.y - halfh,
        center.x + halfw,
        center.y + halfh
    };
}

UIRect CenterUIRectOnUIRect(UIRect base, UIRect rect)
{
    UIPoint center = GetCenterUIRect(base);
    return SetCenterUIRect(rect, center);
}

// -----------------------------------------------------------------------------

ScreenTransform GetScreenTransform(int w, int h, int ow, int oh)
{
    float scale = min((float)w / ow, (float)h / oh);
    float xstart = ((float)w - ((float)ow * scale)) * 0.5f;
    float ystart = ((float)h - ((float)oh * scale)) * 0.5f;
    return (ScreenTransform) { xstart, ystart, scale };
}

UIPoint ScreenTransformUIPoint(UIPoint point, ScreenTransform t)
{
    return (UIPoint)
    {
        t.xstart + point.x * t.scale,
        t.ystart + point.y * t.scale
    };
}

UIRect ScreenTransformUIRect(UIRect rect, ScreenTransform t)
{
    return (UIRect)
    {
        t.xstart + rect.left,
        t.ystart + rect.top,
        t.xstart + rect.right * t.scale,
        t.ystart + rect.bottom * t.scale
    };
}

UIText ScreenTransformUIText(UIText text, ScreenTransform t)
{
    text.fontSize *= t.scale;
    return text;
}

// -----------------------------------------------------------------------------

void DrawUIRect(UIRect rect, Color color)
{
    DrawRectangleRec(UIRectToRectangle(rect), color);
}

void DrawUIBorder(UIRect rect, float width, Color color)
{
    DrawRectangleLinesEx(UIRectToRectangle(rect), width, color);
}

void DrawUIText(UIRect rect, UIText text, UIAlign align)
{
    float spacing = text.fontSize * FONT_SIZE_SPACING_FACTOR;
    UISize textSize = GetUITextSize(text);
    UIPoint loc = { 0, 0 };
    UIPoint center = GetCenterUIRect(rect);

    if (align.halign == H_CENTER)
        loc.x = center.x - (textSize.w * 0.5f);
    else if (align.halign == H_LEFT)
        loc.x = rect.left + align.leftMargin;
    else if (align.halign == H_RIGHT)
        loc.x = rect.right - textSize.w - align.rightMargin;

    if (align.valign == V_CENTER)
        loc.y = center.y - (textSize.h * 0.5f);
    else if (align.valign == V_TOP)
        loc.y = rect.top + align.topMargin;
    else if (align.valign == V_BOTTOM)
        loc.y = rect.bottom - textSize.h - align.bottomMargin;

    size_t strSize = strlen(text.str) + 1;
    char* str = alloca(strSize);
    strncpy(str, text.str, strSize);

    char* token = NULL;
    for (token = strtok(str, "\n"); token != NULL; token = strtok(NULL, "\n"))
    {
        DrawTextEx(
            GetFontDefault(),
            token,
            UIPointToVector2(loc),
            text.fontSize,
            spacing,
            text.fontColor);
        loc.y += text.fontSize + spacing;
    }
}

// -----------------------------------------------------------------------------

/*
typedef struct UIElement
{
    UIRect rect;
    Color bgColor;
    float borderWidth;
    Color borderColor;
    bool hasTexture;
    UIRect textureRect;
    Texture2D texture;
    bool hasText;
    UIText text;
    UIAlign textAlign;
    uint32_t optState;
} UIElement;
*/

UIElement* CreateEmptyUIElement()
{
    UIElement* ret = (UIElement*)malloc(sizeof(UIElement));
    memset(ret, 0, sizeof(UIElement));
    return ret;
}

UIElement* CreateSolidRect(UIRect rect, UIStyle style)
{
    UIElement* ret = CreateEmptyUIElement();
    ret->rect = rect;
    ret->bgColor = style.bgColor;
    ret->borderWidth = style.borderWidth;
    ret->borderColor = style.borderColor;
    ret->hasTexture = false;
    ret->hasText = false;
    return ret;
}

UIElement* CreateLabel(UIRect rect, char* text, float fontSize, UIAlign textAlign, UIStyle style)
{
    UIElement* ret = CreateEmptyUIElement();
    ret->rect = rect;
    ret->bgColor = style.bgColor;
    ret->borderWidth = style.borderWidth;
    ret->borderColor = style.borderColor;
    ret->hasTexture = false;
    ret->hasText = true;
    ret->text = (UIText) { text, fontSize, style.fontColor };
    ret->textAlign = textAlign;
    ret->optState = 0;
    return ret;
}

UIElement* CreateTextureElement(UIRect rect, UIRect textureRect, Texture2D texture, UIStyle style);
UIElement* CreateButton(UIRect rect, char* text, float fontSize, UIAlign textAlign, UIStyle style);
UIElement* CreateButtonWithTexture(UIRect rect, UIRect textureRect, Texture2D texture, UIStyle style);
UIElement* CreateButtonWithTextureAndText(
    UIRect rect, char* text, float fontSize, UIAlign textAlign,
    UIRect textureRect, Texture2D texture, UIStyle style);
UIElement* CreateToggleButton(UIRect rect, char* text, float fontSize, UIAlign textAlign, bool isToggled, UIStyle style);
UIElement* CreateToggleButtonWithTexture(UIRect rect, UIRect textureRect, Texture2D texture, bool isToggled, UIStyle style);
UIElement* CreateToggleButtonWithTextureAndText(
    UIRect rect, char* text, float fontSize, UIAlign textAlign,
    UIRect textureRect, Texture2D texture, bool isToggled, UIStyle style);

// -----------------------------------------------------------------------------

void ScreenTransformUIElement(const UIElement* elem, ScreenTransform t, UIElement* res)
{
    *res = *elem;
    res->rect = ScreenTransformUIRect(elem->rect, t);
    res->borderWidth *= t.scale;
    res->textureRect = ScreenTransformUIRect(elem->rect, t);
    res->text = ScreenTransformUIText(elem->text, t);
}

void DrawUIElement(const UIElement* elem)
{
    UISize rectSize = GetUIRectSize(elem->rect);
    if (rectSize.w > 0 && rectSize.h > 0)
    {
        DrawUIRect(elem->rect, elem->bgColor);
    }

    if (elem->hasTexture)
    {
        DrawTexturePro(
            elem->texture,
            (Rectangle) { 0, 0, elem->texture.width, elem->texture.height },
            UIRectToRectangle(elem->textureRect),
            (Vector2) { 0, 0 },
            0.0f,
            WHITE);
    }

    if (elem->text.str != NULL)
    {
        DrawUIText(elem->rect, elem->text, elem->textAlign);
    }

    if (elem->borderWidth > 0)
    {
        DrawUIBorder(elem->rect, elem->borderWidth, elem->borderColor);
    }
}

void DeleteUIElement(UIElement* elem)
{
    free(elem);
    elem = NULL;
}

// -----------------------------------------------------------------------------

UIScene* CreateUIScene();
void InsertIntoUIScene(UIElement* elem);
void DeleteFromUIScene(UIElement* elem);
void DrawUIScene(const UIScene* scene);
void DeleteUIScene(UIScene* scene);