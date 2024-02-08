#ifndef TILEMAP_H
#define TILEMAP_H
#include <iostream>
#include "tile.h"
#include "utils.h"

class Tilemap {
    public:
        //x and y are where items are drawn relative to eachother
        //in this case we're drawing the top left corner of the spritesheet at 0,0
        float x = 0;
        float y = 0;
        // 48 x 48 of (32x32) tiles
        int widthTiles = 48;
        int heightTiles = 48;
        Tile tileMapFG[48][48];
        Tile tileMap[48][48];

        int row = 0;
        int column = 0;
        Tilemap();
        void init(Tile (&tilemap)[48][48], std::string filename);


        void update(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen, bool isToolActive, bool tbClicked, bool traceMode, bool floodMode, bool eraserMode);
        void handleAClick(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen, bool traceMode, bool floodMode, bool eraserMode);

        void outputMap(Tile (&tilemap)[48][48], std::string filename);

        void draw(int xViewpoint, int yViewpoint, Texture2D spritesheetTex, Vector2 selected);

        void drawEditMode();

        void drawGrid(int xViewpoint, int yViewpoint);

        bool detectMapClicked(int xViewpoint, int yViewpoint);

        bool calculateSquareClicked(int xViewpoint, int yViewpoint);

        void drawSprite(int xViewpoint, int yViewpoint);
};
#endif