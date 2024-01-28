#include <iostream>
#include "../inc/raylib.h"
#include <vector>
#include <random>

#define GRID_COUNT 32
#define GRID_SIZE 32

double startTime = 0;
double currentTime = 0;

bool timePassed(double elapsed);

class Viewpoint {
    public:
    // viewpoint x and y is added to all drawn items
        int x{0};
        int y{0}; 

        void move() {
            if(IsKeyDown(KEY_W)) {
                y += 10;
            } 
            if(IsKeyDown(KEY_S)) {
                y -= 10;
            }
            if(IsKeyDown(KEY_A)) {
                x += 10;
            }
            if(IsKeyDown(KEY_D)) {
                x -= 10;
            }
        }
};

class Spritesheet {
    public:

        //x and y are where items are drawn relative to eachother
        //in this case we're drawing the top left corner of the spritesheet at 0,0
        float x = GRID_COUNT*GRID_SIZE/2;
        float y = GRID_COUNT*GRID_SIZE/2;
        float width = 512;
        float height = 512;
        Image spritesheetImg;
        Texture2D spritesheetTex;

        Spritesheet() {
            spritesheetImg = LoadImage("assets/spritesheet.png");
            spritesheetTex = LoadTextureFromImage(spritesheetImg);
        }

        ~Spritesheet() {
            UnloadImage(spritesheetImg);
            UnloadTexture(spritesheetTex);
        }

        // float xScreen = 0;
        // float yScreen = 0;


        void update(int xViewpoint, int yViewpoint) {
            detectSpriteClick(xViewpoint, yViewpoint);
            if (timePassed(1))
            {
                // the origin of the spritesheet + the offset of the viewpoint
                printf("my relative coordinates are %f, %f\n", x+xViewpoint, y+yViewpoint);
            }
        }

        void drawRec(int xViewpoint, int yViewpoint) {
            DrawRectangle(x+xViewpoint, y+yViewpoint, width, height, WHITE);
            DrawTexture(spritesheetTex, x+xViewpoint, y+yViewpoint, WHITE);
        }

        void detectSpriteClick(int xViewpoint, int yViewpoint) {
            Rectangle collisionRec = {x+xViewpoint, y+yViewpoint, width, height};
            Vector2 mousePos = GetMousePosition();
            if(CheckCollisionPointRec(mousePos, collisionRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                calculateSpriteClicked(xViewpoint, yViewpoint);
                std::cout << "clicked" << std::endl << std::flush;
            }
        }

        void calculateSpriteClicked(int xViewpoint, int yViewpoint) {
            Vector2 mousePos = GetMousePosition();
            int xOriginOffset = mousePos.x - (xViewpoint + x);
            int yOriginOffset = mousePos.y - (yViewpoint + y);
            int row = xOriginOffset / GRID_SIZE;
            int column = yOriginOffset / GRID_SIZE;
            // DrawRectangle(xViewpoint+x +(row*GRID_SIZE), yViewpoint+y+(column*GRID_SIZE), GRID_SIZE, GRID_SIZE, BLACK);
        }

        void drawSprite(int xViewpoint, int yViewpoint) {
            // DrawTexture(spritesheetTex, xViewpoint, yViewpoint, WHITE);
        }
};


// need to find a way to move rectangles to match spritesheet
// isntead dont use rectangle map for collisions, just get mouse position, then go up and left until we find a multiple of 32 that is within the spritesheet



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
        std::cout << "clicked" << std::endl << std::flush;
    }
}


int main() {
    InitWindow(GRID_COUNT*GRID_SIZE, GRID_COUNT*GRID_SIZE, "ft2");
    SetWindowPosition(0,0);
    SetTargetFPS(60);

    Viewpoint *viewpoint = new Viewpoint();
    Spritesheet *spritesheet = new Spritesheet();   
    

    while(!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);
        
        viewpoint->move();
        spritesheet->drawRec(viewpoint->x, viewpoint->y);
        spritesheet->update(viewpoint->x, viewpoint->y);
        EndDrawing();
    }
    delete viewpoint;
    delete spritesheet;
    return 0;
}

