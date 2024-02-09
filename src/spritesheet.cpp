#include "spritesheet.h"
Spritesheet::Spritesheet() {
    // center the spritesheet
    x = (GRID_COUNT*GRID_SIZE/2)-(SPRITESHEET_SIZE/2);
    y = (GRID_COUNT*GRID_SIZE/2)-(SPRITESHEET_SIZE/2);

    dimension = sqrt(SPRITESHEET_SIZE/2);
    std::cout << dimension << std::endl;
    spritesheetImg = LoadImage("assets/spritesheet.png");
    spritesheetTex = LoadTextureFromImage(spritesheetImg);
}

Spritesheet::~Spritesheet() {
    UnloadImage(spritesheetImg);
    UnloadTexture(spritesheetTex);
}

void Spritesheet::update() {
    detectSpriteClick();
}

void Spritesheet::draw(bool traceMode) {
    if(traceMode) {
        DrawTexture(spritesheetTex, x, y, Color{255, 255, 255, 200});
        DrawRectangle(x, y, SPRITESHEET_SIZE, SPRITESHEET_SIZE, Color{255, 255, 255, 100});
    }
    else {
        DrawRectangle(x, y, SPRITESHEET_SIZE, SPRITESHEET_SIZE, Color{132, 117, 119, 255});
        DrawTexture(spritesheetTex, x, y, WHITE);
    }
    drawGrid();
}

void Spritesheet::drawGrid() {
    for(int i = 0; i <= dimension; i++) {
        DrawLine(x+(i*GRID_SIZE), y, x+(i*GRID_SIZE), y+SPRITESHEET_SIZE, BLACK);
        DrawLine(x, y+(i*GRID_SIZE), x+SPRITESHEET_SIZE, y+(i*GRID_SIZE), BLACK);
    }
}

void Spritesheet::detectSpriteClick() {
    Rectangle collisionRec = {x, y, SPRITESHEET_SIZE, SPRITESHEET_SIZE};
    Vector2 mousePos = GetMousePosition();
    if(CheckCollisionPointRec(mousePos, collisionRec) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON))) {
        calculateSpriteClicked();
    }
}

void Spritesheet::calculateSpriteClicked() {
    Vector2 mousePos = GetMousePosition();
    int xOriginOffset = mousePos.x - x;
    int yOriginOffset = mousePos.y - y;
    int row = xOriginOffset / GRID_SIZE;
    int column = yOriginOffset / GRID_SIZE;
    selected.x = column;
    selected.y = row;
}