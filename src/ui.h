#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "util.h"
#include "raylib.h"

#define SCREEN_TRANSFORM_NONE (ScreenTransform) { 0, 0, 1 }
#define UIPOINT_ZERO (UIPoint) { 0, 0 }
#define UIRECT_ZERO (UIRect) { 0, 0 }

#define WARMAGIC_STYLE (UIStyle) { BLACK, 4.0f, DARKPURPLE, PURPLE }
#define WARMAGIC_STYLE_NOBORDER (UIStyle) { BLACK, 0.0f, BLANK, PURPLE }

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

typedef struct UISize
{
    float w;
    float h;
} UISize;

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

typedef enum HAlign
{
    H_CENTER, H_LEFT, H_RIGHT
} HAlign;

typedef enum VAlign
{
    V_CENTER, V_TOP, V_BOTTOM
} VAlign;

typedef struct UIAlign
{
    HAlign halign;
    VAlign valign;
    float leftMargin;
    float topMargin;
    float rightMargin;
    float bottomMargin;
} UIAlign;

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

typedef struct UIStyle
{
    Color bgColor;
    float borderWidth;
    Color borderColor;
    Color fontColor;
} UIStyle;

typedef struct UIScene
{
    size_t size;
    UIElement** elements;
} UIScene;

Vector2 UIPointToVector2(UIPoint point);
Rectangle UIRectToRectangle(UIRect rect);

UISize GetUIRectSize(UIRect rect);
UISize GetUITextSize(UIText text);
bool CollidesUIRectUIPoint(UIRect rect, UIPoint point);
UIPoint GetCenterUIRect(UIRect rect);
UIRect SetCenterUIRect(UIRect rect, UIPoint center);
UIRect CenterUIRectOnUIRect(UIRect base, UIRect rect);

ScreenTransform GetScreenTransform(int w, int h, int ow, int oh);
UIPoint ScreenTransformUIPoint(UIPoint point, ScreenTransform t);
UIRect ScreenTransformUIRect(UIRect rect, ScreenTransform t);
UIText ScreenTransformUIText(UIText text, ScreenTransform t);

void DrawUIRect(UIRect rect, Color color);
void DrawUIBorder(UIRect rect, float width, Color color);
void DrawUIText(UIRect rect, UIText text, UIAlign align);

UIElement* CreateEmptyUIElement();
UIElement* CreateSolidRect(UIRect rect, UIStyle style);
UIElement* CreateLabel(UIRect rect, char* text, float fontSize, UIAlign textAlign, UIStyle style);
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

void ScreenTransformUIElement(const UIElement* elem, ScreenTransform t, UIElement* res);
void DrawUIElement(const UIElement* elem);
void DeleteUIElement(UIElement* elem);

UIScene* CreateUIScene();
void InsertIntoUIScene(UIElement* elem);
void DeleteFromUIScene(UIElement* elem);
void DrawUIScene(const UIScene* scene);
void DeleteUIScene(UIScene* scene);

#endif