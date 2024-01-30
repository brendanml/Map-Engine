#include <iostream>
#include "../inc/raylib.h"
#include <vector>
#include <random>
#include <iomanip>

#define GRID_COUNT 32
#define GRID_SIZE 32

#define SPRITESHEET_WIDTH 512
#define SPRITESHEET_HEIGHT 512
#define SPRITESHEET_COLUMNS 16

double startTime = 0;
double currentTime = 0;

bool timePassed(double elapsed);

int rowColEncoder(int row, int col) {
    int number = col * SPRITESHEET_COLUMNS + row + 1;
    return number;
}
Vector2 rowColDecoder(int number) {
    int row = (number - 1) / SPRITESHEET_COLUMNS;
    int col = (number - 1) % SPRITESHEET_COLUMNS;
    return {(float)col, (float)row};
}

class Viewpoint { 
    public:
    // viewpoint x and y is added to all drawn items
        int x{0};
        int y{0};

        void move() {
            if(IsKeyPressed(KEY_W)) {
                y += 32;
            } 
            if(IsKeyPressed(KEY_S)) {
                y -= 32;
            }
            if(IsKeyPressed(KEY_A)) {
                x += 32;
            }
            if(IsKeyPressed(KEY_D)) {
                x -= 32;
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
        float width = SPRITESHEET_WIDTH;
        float height = SPRITESHEET_HEIGHT;
        Image spritesheetImg;
        Texture2D spritesheetTex;

        //set default tile selected
        Vector2 selected = {0,0};

        Spritesheet() {
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
        }

        void draw() {
            DrawRectangle(x, y, width, height, Color{132, 117, 119, 255});
            DrawTexture(spritesheetTex, x, y, WHITE);
            drawGrid();
        }

        void drawGrid() {
            for(int i = 0; i <= SPRITESHEET_WIDTH/GRID_SIZE; i++) {
                DrawLine(x+(i*GRID_SIZE), y, x+(i*GRID_SIZE), y+height, BLACK);
                DrawLine(x, y+(i*GRID_SIZE), x+width, y+(i*GRID_SIZE), BLACK);
            }
        }

        void detectSpriteClick() {
            Rectangle collisionRec = {x, y, width, height};
            Vector2 mousePos = GetMousePosition();
            if(CheckCollisionPointRec(mousePos, collisionRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                calculateSpriteClicked();
            }
        }

        void calculateSpriteClicked() {
            Vector2 mousePos = GetMousePosition();
            int xOriginOffset = mousePos.x - x;
            int yOriginOffset = mousePos.y - y;
            int row = xOriginOffset / GRID_SIZE;
            int column = yOriginOffset / GRID_SIZE;
            selected.x = column;
            selected.y = row;
            int num = rowColEncoder(row, column);
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
        int tileMapFG[48][48];


        int row = 0;
        int column = 0;

        Tilemap() {
            for(int i = 0; i < 48; i++) {
                for(int j = 0; j < 48; j++) {
                    tileMap[i][j] = 0;
                    tileMapFG[i][j] = 0;
                }
            }
        }


        void update(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen) {
            handleClick(xViewpoint, yViewpoint, selected, windowOpen);
        }

        void handleClick(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen) {
            if(!windowOpen) {

                bool clicked = detectMapClicked(xViewpoint, yViewpoint);
                if(clicked && IsKeyDown(KEY_LEFT_SHIFT)) {
                    // printf("the tilemap square clicked is %d, %d\n", row, column);
                    // need to pass selected sprite from ui, then assign its x, y pair to the tilemap position
                    tileMap[row][column] = rowColEncoder(selected.y, selected.x);
                } if(clicked && IsKeyDown(KEY_LEFT_CONTROL)) {
                    std::cout << "LEFT CONTROL CLICKED" << std::endl << std::flush;
                    tileMapFG[row][column] = rowColEncoder(selected.y, selected.x);
                }
            }
        }   

        void printTileMap() {
            // print the new tile maps
                    std::cout << "Background Tile Map" << std::endl;
                    std::cout << "{\n";
                    for (int i = 0; i < 48; ++i) {
                        std::cout << "{";
                        for (int j = 0; j < 48; ++j) {
                            std::cout << tileMap[j][i];
                            // Add comma and space for elements except the last one
                            if (j < 47) {
                                std::cout << ", ";
                            }
                        }
                        std::cout << "}\n";
                        
                        // Add comma and space for rows except the last one
                        if (i < 47) {
                            std::cout << ",";
                        }
                        
                        std::cout << "\n";
                    }
                    std::cout << "}\n";

                    std::cout << "Foreground Tile Map" << std::endl;
                    std::cout << "{\n";
                    for (int i = 0; i < 48; ++i) {
                        std::cout << "{";
                        for (int j = 0; j < 48; ++j) {
                            std::cout << tileMapFG[j][i];
                            // Add comma and space for elements except the last one
                            if (j < 47) {
                                std::cout << ", ";
                            }
                        }
                        std::cout << "}\n";
                        
                        // Add comma and space for rows except the last one
                        if (i < 47) {
                            std::cout << ",";
                        }
                        
                        std::cout << "\n";
                    }
                    std::cout << "}\n";
        }

        void draw(int xViewpoint, int yViewpoint, Texture2D spritesheetTex, Vector2 selected) {
            DrawRectangle(x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE, RAYWHITE);
            // draw tileMap
            BeginBlendMode(BLEND_ALPHA);
            for(int i = 0; i < 48; i++) {
                for(int j = 0; j < 48; j++) {
                    // draw canvas color if nothing there
                    int currentTileNumber = tileMap[i][j];
                    int currentFGTileNumber = tileMapFG[i][j];
                    // if(currentTileNumber == 0) {
                    //     DrawRectangle(x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE), GRID_SIZE, GRID_SIZE, RAYWHITE);       
                    // } else {
                        Vector2 offset = rowColDecoder(currentTileNumber);
                        Vector2 offsetFG = rowColDecoder(currentFGTileNumber);
                        // printf("the offset is row: %f, col: %f\n", offset.y, offset.x);
                        // if(timePassed(1))
                        DrawTextureRec(spritesheetTex, {GRID_SIZE*offset.x, GRID_SIZE*offset.y, GRID_SIZE, GRID_SIZE}, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
                        DrawTextureRec(spritesheetTex, {GRID_SIZE*offsetFG.x, GRID_SIZE*offsetFG.y, GRID_SIZE, GRID_SIZE}, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
                        // draw purple square underneath

                    // }
                }
            }
            EndBlendMode();
            drawGrid(xViewpoint, yViewpoint);
        }

        void drawGrid(int xViewpoint, int yViewpoint) {
            // draw grid that overlays the tilemap
            for(int i = 0; i <= widthTiles; i++) {
                DrawLine((x+xViewpoint)+(i*GRID_SIZE), y+yViewpoint, (x+xViewpoint)+(i*GRID_SIZE), y+yViewpoint+(heightTiles*GRID_SIZE), BLACK);
                DrawLine(x+xViewpoint, (y+yViewpoint)+(i*GRID_SIZE), x+xViewpoint+(widthTiles*GRID_SIZE), y+yViewpoint+(i*GRID_SIZE), BLACK);
            }
        }

        bool detectMapClicked(int xViewpoint, int yViewpoint) {
            Rectangle collisionRec = {x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE};
            Vector2 mousePos = GetMousePosition();
            if(CheckCollisionPointRec(mousePos, collisionRec) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON))) {
                // std::cout << "clicked" << std::endl << std::flush;
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
            // printf("the tilemap tile clicked is %d, %d\n", row, column);
            return true;
            // DrawRectangle(xViewpoint+x +(row*GRID_SIZE), yViewpoint+y+(column*GRID_SIZE), GRID_SIZE, GRID_SIZE, BLACK);
        }

        void drawSprite(int xViewpoint, int yViewpoint) {
            // DrawTexture(spritesheetTex, xViewpoint, yViewpoint, WHITE);

        }
};

class UI {
    public:
        bool windowsOpen[3] = {false, false, false};

        // need a print button for tilemaps


        //index 1 -> controls menu
        // index 2 -> toggle sprite sheet
        // index 3 -> toggle tile map editing


        Spritesheet *spritesheet;
        Tilemap *tilemap;
        UI() {
            tilemap = new Tilemap();
            spritesheet = new Spritesheet();
        }
        ~UI() {
            delete spritesheet;
            delete tilemap;
        }

        void draw(int xViewpoint, int yViewpoint) {
            tilemap->draw(xViewpoint, yViewpoint, spritesheet->spritesheetTex, spritesheet->selected);
            if(windowsOpen[0]) {
                spritesheet->draw();
            }
        }

        void update(int xViewpoint, int yViewpoint) {
            tilemap->update(xViewpoint, yViewpoint, spritesheet->selected, windowsOpen[0]);
            if(windowsOpen[0]) {
                spritesheet->update();
            }
            if(IsKeyPressed(KEY_ONE)) {
                windowsOpen[0] = !windowsOpen[0];
            }
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
        // std::cout << "clicked" << std::endl << std::flush;
    }
}


int main() {
    InitWindow(GRID_COUNT*GRID_SIZE, GRID_COUNT*GRID_SIZE, "ft2");
    SetWindowPosition(0,0);
    SetTargetFPS(60);

    Viewpoint *viewpoint = new Viewpoint();
    UI *ui = new UI();
    

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{229, 230, 228, 255});
        
        // update -> move -> draw
        ui->update(viewpoint->x, viewpoint->y);
        viewpoint->move();
        ui->draw(viewpoint->x, viewpoint->y);
        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            ui->tilemap->printTileMap();
        }



        
        EndDrawing();
    }
    delete ui;
    delete viewpoint;
    return 0;
}

