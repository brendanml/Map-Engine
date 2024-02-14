#include "toolbar.h"
Toolbar::Toolbar() {
    width = 56;
    height = 0;
    x = GRID_COUNT*GRID_SIZE - width - offset;
    y = offset;
    constructButtons();
    select = &buttons[0];
    flood = &buttons[1];
    eraser = &buttons[2];
}

Toolbar::~Toolbar() {
    UnloadTexture(uiTex);
}

void Toolbar::constructButtons() {
    int buttonOffset = 4;
    int buttonVerticalOffset = buttonOffset;
    int buttonDimension = width-(2*buttonOffset);
    Image uiImg = LoadImage("assets/uisheet.png");
    uiTex = LoadTextureFromImage(uiImg);
    // here were priming this button to be the first active tool
    buttons.push_back(Button(buttonDimension, x+buttonOffset, y+buttonVerticalOffset, uiTex, 0, 0, true));
    buttonCount++;
    buttonVerticalOffset += buttonDimension + buttonOffset;
    buttons.push_back(Button(buttonDimension, x+buttonOffset, y+buttonVerticalOffset, uiTex, 32, 0, false));
    buttonCount++;
    buttonVerticalOffset += buttonDimension + buttonOffset;
    buttons.push_back(Button(buttonDimension, x+buttonOffset, y+buttonVerticalOffset, uiTex, 64, 0, false));
    buttonCount++;
    buttonVerticalOffset += buttonDimension + buttonOffset;
    actions.push_back(Action(buttonDimension, x+buttonOffset, y+buttonVerticalOffset, uiTex, 96, 0, "save"));

    
    //calc the toolbar height relative to the buttons
    height = buttonVerticalOffset + buttonDimension + buttonOffset;
}

void Toolbar::update(Tilemap tilemap) {
    for(auto& button : buttons) {
        button.update();
        if(button.detectClicked()) {
            resetButtons();
            button.isToolActive = true;
        }
    }
    for(auto& action : actions) {
        action.update();
        if(action.detectClicked()) {
            action.perform(tilemap);
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
    if(CheckCollisionPointRec(mousePos, collisionRec) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) 
    || IsMouseButtonDown(MOUSE_LEFT_BUTTON))) return true;
    return false;
}

void Toolbar::draw() {
    DrawRectangle(x, y, width, height, Color{209, 213, 222, 255});
    DrawRectangleLines(x, y, width, height, BLACK);
    for(auto& button : buttons) {
        button.draw();
    }
    for(auto& action : actions) {
        action.draw();
    }
}