#include "tilemap.h"
#include <fstream>

// LOTS OF PLACES WERE JUST USING INTS FOR THE SIZE OF ARRAYS, WOULD BE A GOOD IDEA TO USE DYNAMIC ARRAYS

Tilemap::Tilemap() {
    init(tileMap, "tilemap.txt");
    init(tileMapFG, "fg.txt");
}

void Tilemap::init(Tile (&tilemap)[48][48], std::string filename) {
    //in is the name of the opened file
    std::ifstream in(filename);

    if(!in.is_open()) { std::cerr << "error opening file" << std::endl; }

    float tileNum{0};
    // let i be the row
    int i = 0;
    // let j be the column
    int j = 0;
    while(in >> tileNum) {
        float offsetX, offsetY;
        in >> offsetX >> offsetY;
        Tile newTile(Vector2{offsetX, offsetY}, tileNum);
        tilemap[i][j] = newTile;
        j++;
        if(j == 48) {
            j = 0;
            i++;
        }
    }
}

void Tilemap::update(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen, bool isToolActive, bool tbClicked, bool traceMode, bool floodMode, bool eraserMode) {
    if((isToolActive || floodMode || eraserMode) && !tbClicked) {
        handleAClick(xViewpoint, yViewpoint, selected, windowOpen, traceMode, floodMode, eraserMode);
    }
}

void Tilemap::handleAClick(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen, bool traceMode, bool floodMode, bool eraserMode) {
    if(!windowOpen || traceMode) {
        bool clicked = detectMapClicked(xViewpoint, yViewpoint);
        if(clicked && IsKeyDown(KEY_LEFT_SUPER)) {
            // printf("the tilemap square clicked is %f, %f, numer: %d\n", newTile.row, newTile.column, newTile.number);
            Tile newTile(Vector2{selected.y, selected.x}, rowColEncoder(selected.y, selected.x));
            tileMapFG[row][column] = newTile;
            if(floodMode) {
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 4; j++) {
                        int rowIndex = row+i;
                        int columnIndex = column+j;
                        if(rowIndex < 48 && columnIndex < 48) {
                            tileMap[rowIndex][columnIndex] = newTile;
                        }
                    }
                }
            }
            if(eraserMode) {
                Tile newTile = Tile();
                if(IsKeyDown(KEY_Z)) {
                    for(int i = 0; i < 4; i++) {
                        for(int j = 0; j < 4; j++) {
                            int rowIndex = row+i;
                            int columnIndex = column+j;
                            if(rowIndex < 48 && columnIndex < 48) {
                                tileMapFG[rowIndex][columnIndex] = newTile;
                            }
                        }
                }
                } else {
                    tileMapFG[row][column] = Tile();
                }
            }
        }
        else if(clicked) {
            // printf("the tilemap square clicked is %d, %d\n", row, column);
            // need to pass selected sprite from ui, then assign its x, y pair to the tilemap position
            Tile newTile(Vector2{selected.y, selected.x}, rowColEncoder(selected.y, selected.x));
            tileMap[row][column] = newTile;
            if(floodMode) {
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 4; j++) {
                        int rowIndex = row+i;
                        int columnIndex = column+j;
                        if(rowIndex < 48 && columnIndex < 48) {
                            tileMap[rowIndex][columnIndex] = newTile;
                        }
                    }
                }
            }
            if(eraserMode) {
                Tile newTile = Tile();
                if(IsKeyDown(KEY_Z)) {
                    for(int i = 0; i < 4; i++) {
                        for(int j = 0; j < 4; j++) {
                            int rowIndex = row+i;
                            int columnIndex = column+j;
                            if(rowIndex < 48 && columnIndex < 48) {
                                tileMap[rowIndex][columnIndex] = newTile;
                            }
                        }
                }
                } else {
                    tileMap[row][column] = Tile();
                }
            }
        }
    }
}

void Tilemap::outputMap(Tile (&tilemap)[48][48], std::string filename) {
    std::ofstream file;
    file.open(filename);
    for(int i = 0; i < widthTiles; i++) {
        for(int j = 0; j < heightTiles; j++) {
            file << tilemap[i][j].number << " " << tilemap[i][j].offset.x << " " << tilemap[i][j].offset.y << std::endl;
        }
    }
    file.close();
    std::cout << "saving your map..." << std::endl << std::flush;
}

void Tilemap::draw(int xViewpoint, int yViewpoint, Texture2D spritesheetTex, Vector2 selected) {

    // draw background
    DrawRectangle(x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE, RAYWHITE);
    // draw tileMap
    BeginBlendMode(BLEND_ALPHA);
    for(int i = 0; i < 48; i++) {
        for(int j = 0; j < 48; j++) {
            // draw canvas color if nothing there
            int currentTileNumber = tileMap[i][j].number;
            if(currentTileNumber != 0 && !IsKeyDown(KEY_C)) {
                Vector2 offset = tileMap[i][j].offset;
                DrawTextureRec(spritesheetTex, {GRID_SIZE*offset.x, GRID_SIZE*offset.y, GRID_SIZE, GRID_SIZE}, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
            }
            int currentFGTileNumber = tileMapFG[i][j].number;
            if(currentFGTileNumber != 0 && !IsKeyDown(KEY_X)) { 
                Vector2 offsetFG = tileMapFG[i][j].offset;
                DrawTextureRec(spritesheetTex, {GRID_SIZE*offsetFG.x, GRID_SIZE*offsetFG.y, GRID_SIZE, GRID_SIZE}, {x+xViewpoint+(i*GRID_SIZE), y+yViewpoint+(j*GRID_SIZE)}, WHITE);
            }
        }
    }
    EndBlendMode();
    drawGrid(xViewpoint, yViewpoint);
    drawEditMode();
}

void Tilemap::drawEditMode() {
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

void Tilemap::drawGrid(int xViewpoint, int yViewpoint) {
    // draw grid that overlays the tilemap
    for(int i = 0; i <= widthTiles; i++) {
        DrawLine((x+xViewpoint)+(i*GRID_SIZE), y+yViewpoint, (x+xViewpoint)+(i*GRID_SIZE), y+yViewpoint+(heightTiles*GRID_SIZE), BLACK);
        DrawLine(x+xViewpoint, (y+yViewpoint)+(i*GRID_SIZE), x+xViewpoint+(widthTiles*GRID_SIZE), y+yViewpoint+(i*GRID_SIZE), BLACK);
    }
}

bool Tilemap::detectMapClicked(int xViewpoint, int yViewpoint) {
    Rectangle collisionRec = {x+xViewpoint, y+yViewpoint, (float)widthTiles*GRID_SIZE, (float)heightTiles*GRID_SIZE};
    Vector2 mousePos = GetMousePosition();
    if(CheckCollisionPointRec(mousePos, collisionRec) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON))) {
        // std::cout << "clicked" << std::endl << std::flush;
        return calculateSquareClicked(xViewpoint, yViewpoint);
    }
    return false;
}

bool Tilemap::calculateSquareClicked(int xViewpoint, int yViewpoint) {
    Vector2 mousePos = GetMousePosition();
    int xOriginOffset = mousePos.x - (xViewpoint + x);
    int yOriginOffset = mousePos.y - (yViewpoint + y);
    row = xOriginOffset / GRID_SIZE;
    column = yOriginOffset / GRID_SIZE;
    // printf("the tilemap tile clicked is %d, %d\n", row, column);
    return true;
    // DrawRectangle(xViewpoint+x +(row*GRID_SIZE), yViewpoint+y+(column*GRID_SIZE), GRID_SIZE, GRID_SIZE, BLACK);
}

void Tilemap::drawSprite(int xViewpoint, int yViewpoint) {
    // DrawTexture(spritesheetTex, xViewpoint, yViewpoint, WHITE);

}
