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
        float x;
        float y;

        //size of the spritesheet
        float width = 512;
        float height = 512;
        Image spritesheetImg;
        Texture2D spritesheetTex;

        //set default tile selected
        Vector2 selected = {0,0};

        Spritesheet() {
            // origin top, left, of spritesheet to center
            x = (GRID_COUNT*GRID_SIZE/2)-(width/2);
            y = (GRID_COUNT*GRID_SIZE/2)-(height/2);

            spritesheetImg = LoadImage("assets/spritesheet.png");
            spritesheetTex = LoadTextureFromImage(spritesheetImg);
        }

        ~Spritesheet() {
            UnloadImage(spritesheetImg);
            UnloadTexture(spritesheetTex);
        }

        void update() {
            detectSpriteClick();
            if (timePassed(1))
            {
                // the origin of the spritesheet + the offset of the viewpoint
                printf("my coordinates are %f, %f\n", x, y);
            }
        }

        void drawRec() {
            DrawRectangle(x, y, width, height, WHITE);
            DrawTexture(spritesheetTex, x, y, WHITE);
        }

        void detectSpriteClick() {
            Rectangle collisionRec = {x, y, width, height};
            Vector2 mousePos = GetMousePosition();
            if(CheckCollisionPointRec(mousePos, collisionRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                calculateSpriteClicked();
                std::cout << "clicked" << std::endl << std::flush;
            }
        }

        void calculateSpriteClicked() {
            Vector2 mousePos = GetMousePosition();
            int xOriginOffset = mousePos.x - x;
            int yOriginOffset = mousePos.y - y;
            int row = xOriginOffset / GRID_SIZE;
            int column = yOriginOffset / GRID_SIZE;
            printf("the square clicked is %d, %d\n", row, column);
            selected.x = row;
            selected.y = column;
            // DrawRectangle(xViewpoint+x +(row*GRID_SIZE), yViewpoint+y+(column*GRID_SIZE), GRID_SIZE, GRID_SIZE, BLACK);
        }

        void drawSprite(int xViewpoint, int yViewpoint) {
            // DrawTexture(spritesheetTex, xViewpoint, yViewpoint, WHITE);
        }
};

class UI {
    public:
        bool windowsOpen[3] = {false, false, false};

        //index 0 is the spritesheet
        Spritesheet *spritesheet;
        UI() {
            spritesheet = new Spritesheet();
        }
        ~UI() {
            delete spritesheet;
        }

        void draw() {
            if(windowsOpen[0]) {
                spritesheet->update();
                spritesheet->drawRec();
            }
        }

        void update() {
            if(IsKeyPressed(KEY_ONE)) {
                windowsOpen[0] = !windowsOpen[0];
            }
        }
};

class Tilemap {
    public:

        //x and y are where items are drawn relative to eachother
        //in this case we're drawing the top left corner of the spritesheet at 0,0
        float x = 0;
        float y = 0;
        // 48 x 48 of (32x32) tiles
        int widthTiles = 48;
        int heightTiles = 48;
        int tileMap[48][48];

        int row = 0;
        int column = 0;

        Tilemap() {
            //initialize tilemap with 0's
            for(int i = 0; i < 48; i++) {
                for(int j = 0; j < 48; j++) {
                    tileMap[i][j] = 0;
                    // if(i == 4) {
                    //     tileMap[i][j] = 1;
                    // }
                }
            }
        }


        void update(int xViewpoint, int yViewpoint) {
            bool clicked = detectMapClicked(xViewpoint, yViewpoint);
            if(clicked) {
                printf("*****the square clicked is %d, %d\n", row, column);
                // need to pass selected sprite from ui, then assign its x, y pair to the tilemap position
                tileMap[row][column] = 1;
            }



            if (timePassed(1))
            {
                // the origin of the spritesheet + the offset of the viewpoint
                printf("the maps screen coordinates are %f, %f\n", x+xViewpoint, y+yViewpoint);
            }
        }

        void drawRec(int xViewpoint, int yViewpoint, Texture2D spritesheetTex, Vector2 selected) {
            DrawRectangle(x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE, RED);
            // draw tileMap
            for(int i = 0; i < 48; i++) {
                for(int j = 0; j < 48; j++) {
                    if(tileMap[i][j] == 0) {
                        DrawRectangle(x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE), GRID_SIZE, GRID_SIZE, GREEN);
                    }
                    else if(tileMap[i][j] == 1) {
                        
                        //this doesnt need selected texture, it just draws what in the array... 3d array? containing x -> y offsets to draw, look above for other area ud need to change
                        Rectangle selectedArea = {selected.x*GRID_SIZE, selected.y*GRID_SIZE, GRID_SIZE, GRID_SIZE};
                        DrawTextureRec(spritesheetTex, selectedArea, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
                    }
                }
            }
        }

        bool detectMapClicked(int xViewpoint, int yViewpoint) {
            Rectangle collisionRec = {x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE};
            Vector2 mousePos = GetMousePosition();
            if(CheckCollisionPointRec(mousePos, collisionRec) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON))) {
                std::cout << "clicked" << std::endl << std::flush;
                return calculateSquareClicked(xViewpoint, yViewpoint);
            }
            return false;
        }

        bool calculateSquareClicked(int xViewpoint, int yViewpoint) {
            Vector2 mousePos = GetMousePosition();
            int xOriginOffset = mousePos.x - (xViewpoint + x);
            int yOriginOffset = mousePos.y - (yViewpoint + y);
            row = xOriginOffset / GRID_SIZE;
            column = yOriginOffset / GRID_SIZE;
            printf("the square clicked is %d, %d\n", row, column);
            return true;
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
    Tilemap *tilemap = new Tilemap();
    UI *ui = new UI();
    

    while(!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);
        
        viewpoint->move();
        tilemap->drawRec(viewpoint->x, viewpoint->y, ui->spritesheet->spritesheetTex, ui->spritesheet->selected);   
        tilemap->update(viewpoint->x, viewpoint->y);
        ui->update();
        ui->draw();


        
        EndDrawing();
    }
    delete viewpoint;
    delete ui;
    return 0;
}

