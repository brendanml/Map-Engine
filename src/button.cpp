#include "button.h"
Button::Button() {
}
Button::Button(int dimension, int x, int y, Texture2D uiTex, int texXOffset, int texYOffset, bool isToolActive) 
: x(x), y(y), dimension(dimension), uiTex(uiTex), texXOffset(texXOffset), texYOffset(texYOffset), isToolActive(isToolActive){
}
void Button::update() {
    //some stuff
}

void Button::draw() {
    if(isToolActive) {
        DrawRectangle(x, y, dimension, dimension, GRAY);
    } else {
        DrawRectangle(x, y, dimension, dimension, WHITE);
    }
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