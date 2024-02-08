# Tile Map Editor
- Tile Map editor serves one core purpose; to provide raylib game makers a way to create and visualize the 2D tile maps of their games.
- Currently in development! There is lots more planned.

## Controls
- Toggle spritesheet View: 1
- Camera control/movement: WASD
- Select sprite from sheet: click
- Place texture: click
- Place texture foreground: cmd + click
- Places lots of texture: flood tool
- Print Tile maps: 4 key
- eraser tool: Click
  - Big erase: hold Z
- view only background: hold X
- view only foreground: hold C

## What is it?
- At a high level, tile map editor takes in a 512x512 spritesheet consisting of 32 x 32 sprites. Once you have created your world by placing the tiles to create your game it will produce the corresponding 2D array of the integer mapped textures.

## Intricacies
- Tiles of the spritesheet are numbered starting at 1, reading left to right, to 256.

## Smart encoding/decoding