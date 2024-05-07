#include "button.h"
Button::Button() {
}
Button::Button(int dimension, int x, int y, Texture2D uiTex, int texXOffset, int texYOffset, bool isToolActive) 
: x(x), y(y), dimension(dimension), uiTex(uiTex), texXOffset(texXOffset), texYOffset(texYOffset), isToolActive(isToolActive){
}
void Button::update() {
    currentlyActiveColor = isToolActive ? activeColor : inactiveColor;
}

void Button::draw() {
    DrawRectangle(x, y, dimension, dimension, currentlyActiveColor);
    DrawRectangleLines(x, y, dimension, dimension, BLACK);
    DrawTextureRec(uiTex, {texXOffset, texYOffset, texSize, texSize}, {x+imgOffset, y+imgOffset}, WHITE);
}

bool Button::detectClicked() {
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(GetMousePosition(), {x, y, dimension, dimension})) {
            return true;
        }
    }
    return false;
}