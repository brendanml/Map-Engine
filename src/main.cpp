#include <iostream>
#include "../inc/raylib.h"
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>

#define GRID_COUNT 32
#define GRID_SIZE 32

#define SPRITESHEET_SIZE 512
#define SPRITESHEET_COLUMNS 16
#define SPRITESHEET_ROWS 16

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

class Tile {
    public:
        Vector2 offset = {0, 0};
        int number = 0;

    Tile() {

    }  

    Tile (Vector2 offset, int number) : offset(offset), number(number) {

    }
};

class Viewpoint { 
    public:
    // viewpoint x and y is added to all drawn items
        int x{0};
        int y{0};
        int speed = 1;

        void move() {
            if(IsKeyDown(KEY_LEFT_SHIFT)) {    
                speed = 6;
            } else {
                speed = 1;
            }
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

};
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
        Spritesheet() {
            // center the spritesheet
            x = SPRITESHEET_SIZE-(SPRITESHEET_SIZE/2);
            y = (SPRITESHEET_SIZE)-(SPRITESHEET_SIZE/2);

            dimension = sqrt(SPRITESHEET_SIZE/2);
            std::cout << dimension << std::endl;
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
            DrawRectangle(x, y, SPRITESHEET_SIZE, SPRITESHEET_SIZE, Color{132, 117, 119, 255});
            DrawTexture(spritesheetTex, x, y, WHITE);
            drawGrid();
        }

        void drawGrid() {
            for(int i = 0; i <= dimension; i++) {
                DrawLine(x+(i*GRID_SIZE), y, x+(i*GRID_SIZE), y+SPRITESHEET_SIZE, BLACK);
                DrawLine(x, y+(i*GRID_SIZE), x+SPRITESHEET_SIZE, y+(i*GRID_SIZE), BLACK);
            }
        }

        void detectSpriteClick() {
            Rectangle collisionRec = {x, y, SPRITESHEET_SIZE, SPRITESHEET_SIZE};
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
        Tile tileMapFG[48][48];
        Tile tileMap[48][48];

        int row = 0;
        int column = 0;


        void update(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen) {
            handleClick(xViewpoint, yViewpoint, selected, windowOpen);
        }

        void handleClick(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen) {
            if(!windowOpen) {

                bool clicked = detectMapClicked(xViewpoint, yViewpoint);
                if(clicked && IsKeyDown(KEY_LEFT_SUPER)) {
                    // printf("the tilemap square clicked is %f, %f, numer: %d\n", newTile.row, newTile.column, newTile.number);
                    std::cout << "LEFT CMD CLICKED" << std::endl << std::flush;
                    Tile newTile(Vector2{selected.y, selected.x}, rowColEncoder(selected.y, selected.x));
                    tileMapFG[row][column] = newTile;
                }
                else if(clicked) {
                    // printf("the tilemap square clicked is %d, %d\n", row, column);
                    // need to pass selected sprite from ui, then assign its x, y pair to the tilemap position
                    Tile newTile(Vector2{selected.y, selected.x}, rowColEncoder(selected.y, selected.x));
                    tileMap[row][column] = newTile;
                    // tileMap[row][column] = rowColEncoder(selected.y, selected.x);
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
                            std::cout << tileMap[j][i].number;
                            // Add comma and space for elements except the last one
                            if (j < 47) {
                                std::cout << ", ";
                            }
                        }
                        std::cout << "}";
                        
                        // Add comma and space for rows except themak last one
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
                            std::cout << tileMapFG[j][i].number;
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

            // draw background
            DrawRectangle(x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE, RAYWHITE);
            // draw tileMap
            BeginBlendMode(BLEND_ALPHA);
            for(int i = 0; i < 48; i++) {
                for(int j = 0; j < 48; j++) {
                    // draw canvas color if nothing there
                    int currentTileNumber = tileMap[i][j].number;
                    if(currentTileNumber != 0) {
                        Vector2 offset = tileMap[i][j].offset;
                        DrawTextureRec(spritesheetTex, {GRID_SIZE*offset.x, GRID_SIZE*offset.y, GRID_SIZE, GRID_SIZE}, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
                    }
                    int currentFGTileNumber = tileMapFG[i][j].number;
                    if(currentFGTileNumber != 0) { 
                        Vector2 offsetFG = tileMapFG[i][j].offset;
                        DrawTextureRec(spritesheetTex, {GRID_SIZE*offsetFG.x, GRID_SIZE*offsetFG.y, GRID_SIZE, GRID_SIZE}, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
                    }
                }
            }
            EndBlendMode();
            drawGrid(xViewpoint, yViewpoint);
            drawEditMode();
        }

        void drawEditMode() {
            if(IsKeyDown(KEY_LEFT_SUPER)) {
                DrawRectangle(0, 0, 160, 32, Color{209, 213, 222, 255});
                DrawRectangleLines(0, 0, 160, 32, BLACK);
                DrawText("Foreground", 10, 10, 20, BLACK);
            } else {
                DrawRectangle(0, 0, 160, 32, Color{209, 213, 222, 255});
                DrawRectangleLines(0, 0, 160, 32, BLACK);
                DrawText("Background", 10, 10, 20, BLACK);
            }
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


        Spritesheet spritesheet;
        Tilemap tilemap;

        void draw(int xViewpoint, int yViewpoint) {
            tilemap.draw(xViewpoint, yViewpoint, spritesheet.spritesheetTex, spritesheet.selected);
            if(windowsOpen[0]) {
                spritesheet.draw();
            }
        }

        void update(int xViewpoint, int yViewpoint) {
            tilemap.update(xViewpoint, yViewpoint, spritesheet.selected, windowsOpen[0]);
            if(windowsOpen[0]) {
                spritesheet.update();
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

    Viewpoint viewpoint;
    UI ui; 
    

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{229, 230, 228, 255});
        
        // update -> move -> draw
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

