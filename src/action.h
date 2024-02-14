#ifndef ACTION_H
#define ACTION_H
#include "button.h"
class Action : public Button {
    public:
        std::string function;
        Action();
        Action(int dimension, int x, int y, Texture2D uiTex, int texXOffset, int texYOffset, std::string action);
        void perform(Tilemap tilemap);
};
#endif