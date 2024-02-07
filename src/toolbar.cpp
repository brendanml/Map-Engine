#include "toolbar.h"
Toolbar::Toolbar() {
    width = 56;
    height = GRID_COUNT*GRID_SIZE-(offset*2);
    x = GRID_COUNT*GRID_SIZE - width - offset;
    y = offset;
    constructButtons();
    select = &buttons[0];
}

Toolbar::~Toolbar() {
    UnloadTexture(uiTex);
}

void Toolbar::constructButtons() {
    int buttonOffset = 4;
    int currentButtonOffset = buttonOffset;
    int buttonDimension = width-(2*buttonOffset);
    Image uiImg = LoadImage("assets/uisheet.png");
    uiTex = LoadTextureFromImage(uiImg);
    buttons.push_back(Button(buttonDimension, x+buttonOffset, y+currentButtonOffset, uiTex, 0, 0));
    buttonCount++;
    currentButtonOffset += buttonDimension + currentButtonOffset;
    buttons.push_back(Button(buttonDimension, x+buttonOffset, y+currentButtonOffset, uiTex, 32, 0));
    buttonCount++;
}

void Toolbar::update() {
    for(auto& button : buttons) {
        button.update();
        if(button.detectClicked()) {
            resetButtons();
            button.isToolActive = true;
        }
    }
}

void Toolbar::resetButtons() {
    for(auto& button : buttons) {
        button.isToolActive = false;
    }
}

bool Toolbar::detectClicked() {
    Rectangle collisionRec = {x, y, width, height};
    Vector2 mousePos = GetMousePosition();
    if(CheckCollisionPointRec(mousePos, collisionRec) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON))) {
        return true;
    }
    return false;
}

void Toolbar::draw() {
    DrawRectangle(x, y, width, height, Color{209, 213, 222, 255});
    DrawRectangleLines(x, y, width, height, BLACK);
    for(auto& button : buttons) {
        button.draw();
    }
}