#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "button.h"
#include "action.h"
#include "utils.h"
#include "tilemap.h"
class Toolbar {
    public:
        float x;
        float y;
        float width;
        float height;
        int offset = 12;
        Texture2D uiTex;
        int buttonCount = 0;
        std::vector<Button> buttons;
        std::vector<Action> actions;
        int selectedButton = 1;
        Button *select;
        Button *flood;
        Button *eraser;
        Toolbar();
        ~Toolbar();
        void constructButtons();

        void update(Tilemap tilemap);

        void resetButtons();

        bool detectClicked();

        void draw();
};
#endif