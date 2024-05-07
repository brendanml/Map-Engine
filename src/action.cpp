#include "action.h"
// class extends button, as these aren't toggleable tools, rather actions to be performed
Action::Action() {
    //
}

Action::Action(int dimension, int x, int y, Texture2D uiTex, int texXOffset, int texYOffset, std::string action) 
: Button(dimension, x, y, uiTex, texXOffset, texYOffset, false), function(action) {
    lastClickedTime = GetTime();
}

void Action::update() {
    // currentlyActiveColor = isClicked ? activeColor : inactiveColor;
}


void Action::perform(Tilemap tilemap) {
    if(function == "save") {
        tilemap.outputMap(tilemap.tileMap, std::string("tilemap.txt"));
        tilemap.outputMap(tilemap.tileMapFG, std::string("fg.txt"));
    }
}