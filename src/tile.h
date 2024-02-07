#ifndef TILE_H
#define TILE_H
#include "utils.h"

class Tile {
    public:
        Vector2 offset = {0, 0};
        int number = 0;

        Tile();
        Tile (Vector2 offset, int number);
};
#endif