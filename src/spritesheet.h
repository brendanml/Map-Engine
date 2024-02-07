#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include <iostream>
#include "utils.h"
class Spritesheet {
    public:

        //x and y are where items are drawn relative to eachother
        //in this case we're drawing the top left corner of the spritesheet at 0,0
        float x;
        float y;

        Image spritesheetImg;
        Texture2D spritesheetTex;

        //set default tile selected
        Vector2 selected = {0,0};
        int dimension;

        Spritesheet();

        ~Spritesheet();

        void update();

        void draw(bool traceMode);

        void drawGrid();

        void detectSpriteClick();

        void calculateSpriteClicked();
};


#endif