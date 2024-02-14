#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include "utils.h"
#include "tilemap.h"

class Button {
    public: 
        float x;
        float y;
        float dimension;
        Texture2D uiTex;
        float texSize = 32;
        float imgOffset = 8;
        float texXOffset = 0;
        float texYOffset = 0;
        bool isToolActive = false;
        Button();
        Button(int dimension, int x, int y, Texture2D uiTex, int texXOffset, int texYOffset, bool isToolActive);
        void update();

        void draw();

        bool detectClicked();
};

#endif