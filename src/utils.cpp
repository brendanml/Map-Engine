#include "utils.h"

double startTime = 0;
double currentTime= 0;

int rowColEncoder(int row, int col) {
    int number = col * SPRITESHEET_COLUMNS + row + 1;
    return number;
}
Vector2 rowColDecoder(int number) {
    int row = (number - 1) / SPRITESHEET_COLUMNS;
    int col = (number - 1) % SPRITESHEET_COLUMNS;
    return {(float)col, (float)row};
}

bool timePassed(double elapsed) {
    currentTime = GetTime();
    if(currentTime - startTime > elapsed) {
        // std::cout << currentTime << std::flush;
        // std::cout << startTime << std::flush;
        startTime = currentTime;
        return true;
    }
    return false;
}

void detectRectangleClick(Rectangle collisionRec) {
    Vector2 mousePos = GetMousePosition();
    if(CheckCollisionPointRec(mousePos, collisionRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // std::cout << "clicked" << std::endl << std::flush;
    }
}
