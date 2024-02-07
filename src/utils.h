#ifndef UTILS_H
#define UTILS_H
#include "raylib.h"

#define GRID_COUNT 28
#define GRID_SIZE 32

#define SPRITESHEET_SIZE 512
#define SPRITESHEET_COLUMNS 16
#define SPRITESHEET_ROWS 16

extern double startTime;
extern double currentTime;
extern int rowColEncoder(int row, int col);
extern Vector2 rowColDecoder(int number);
extern bool timePassed(double elapsed);
extern void detectRectangleClick(Rectangle collisionRec);
#endif