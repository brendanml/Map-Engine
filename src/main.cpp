#include <iostream>
#include "../inc/raylib.h"
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>

// my classes
#include "tile.h"
#include "utils.h"
#include "tilemap.h"
#include "spritesheet.h"
#include "toolbar.h"

using namespace std;

class Viewpoint { 
    public:
    // viewpoint x and y is added to all drawn items 
    // testing make speed
        int asd = 0;
        int x{0};
        int y{0};
        int speed = 1;
        bool freeView = true;

        void move() {
            if(IsKeyPressed(KEY_LEFT_SHIFT)) {    
                toggleFreeView();
            }
            if(freeView) {
                if(IsKeyDown(KEY_W)) {
                    y += 12;
                }
                if(IsKeyDown(KEY_S)) {
                    y -= 12;
                }
                if(IsKeyDown(KEY_A)) {
                    x += 12;
                }
                if(IsKeyDown(KEY_D)) {
                    x -= 12;
                }
            } else {
                if(IsKeyPressed(KEY_W)) {
                    y += speed*32;
                } 
                if(IsKeyPressed(KEY_S)) {
                    y -= speed*32;
                }
                if(IsKeyPressed(KEY_A)) {
                    x += speed*32;
                }
                if(IsKeyPressed(KEY_D)) {
                    x -= speed*32;
                }
            }
        }

        void toggleFreeView() {
            freeView = !freeView;
            if(!freeView) {
                x = nearestMultipleOf32(x);
                y = nearestMultipleOf32(y);
            }
        }

        int nearestMultipleOf32(int num) {
            return (num + 16) / 32 * 32;
        }

};

class UI {
    public:
        bool windowsOpen[3] = {false, false, false};
    // asdasd
        Spritesheet spritesheet;
        Tilemap tilemap;
        Toolbar toolbar;
        bool traceMode = false;

        void draw(int xViewpoint, int yViewpoint) {
            tilemap.draw(xViewpoint, yViewpoint, spritesheet.spritesheetTex, spritesheet.selected);
            if(windowsOpen[0]) {
                spritesheet.draw(traceMode);
            }
            toolbar.draw();
        }

        void update(int xViewpoint, int yViewpoint) {
            bool tbClicked = toolbar.detectClicked();
            // if(IsKeyPressed(KEY_TWO)) {
            //     traceMode = !traceMode;
            // }
            toolbar.update();
            tilemap.update(xViewpoint, yViewpoint, spritesheet.selected, windowsOpen[0], toolbar.select->isToolActive, tbClicked, traceMode);
            if(windowsOpen[0]) {
                spritesheet.update();
            }
            if(IsKeyPressed(KEY_ONE)) {
                windowsOpen[0] = !windowsOpen[0];
            }
        }
};

int main() {
    InitWindow(GRID_COUNT*GRID_SIZE, GRID_COUNT*GRID_SIZE, "ft2");
    SetWindowPosition(0,0);
    SetTargetFPS(60);

    Viewpoint viewpoint;
    UI ui; 
    

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{229, 230, 228, 255});
        
        ui.update(viewpoint.x, viewpoint.y);
        viewpoint.move();
        ui.draw(viewpoint.x, viewpoint.y);
        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            ui.tilemap.printTileMap();
        }
        EndDrawing();
    }
    return 0;
}

