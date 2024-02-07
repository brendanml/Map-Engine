#include "tilemap.h"

void Tilemap::update(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen, bool isToolActive, bool tbClicked, bool traceMode) {
    if(isToolActive && !tbClicked) {
        handleAClick(xViewpoint, yViewpoint, selected, windowOpen, traceMode);
    }
}

void Tilemap::handleAClick(int xViewpoint, int yViewpoint, Vector2 selected, bool windowOpen, bool traceMode) {
    if(!windowOpen || traceMode) {
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

void Tilemap::printTileMap() {
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

void Tilemap::draw(int xViewpoint, int yViewpoint, Texture2D spritesheetTex, Vector2 selected) {

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
