# Map Engine
- Map Editor is a tool for game developers. Put simply: It takes in two text files and outputs the edited 

## Why did I build this?
- I started developing this map editor after my first attempt and building a top down 2D RPG without a game engine...
- I was having a lot of fun, but making creative choices (tree placements, crop areas, world design) was proving exceptionally hard to visualize.

## Notes
- Currently only supports 48x48 tile maps and 512x512 spritesheets... This is something I hope to change as soon as possible.

## Controls
- Save Map State: Key [4]
- Toggle spritesheet View: [tab]
- Camera control/movement: [W][A][S][D]
- Select sprite from sheet: [Click] on the spritesheet.
- Place texture: [Click] on location to place.
- Place texture foreground: Hold [cmd] + [Click]
- Places lots of texture: flood tool
- eraser tool: [Click] on tile to be removed.
  - Big erase: hold [Z]
- view only background: Hold Key [X]
- view only foreground: Hold Key [C]
- Snap to grid: [shift]
- Trace Mode: Key [2]
  - This mode is definitely the most complex and I imagine might be difficult to use.
  - How I use it: I snap to grid with [shift], then enable viewing spritesheet [tab]. Then I toggle trace mode with [2]. Now you can trace through the spritesheet onto your map.

## How does it work?
- At a high level, tile map editor takes in a 512x512 spritesheet from the user (name is spritesheet.png, place it in assets) consisting of 256, 32 x 32 sprites.
- Your saved tile maps are saved in 2 files: tilemap.txt (background), and fg.txt (foreground). They will be located in the root of the project directory.
- The format of these tiles is as follows:
  - Each line consists of 3 integers: {TileNum} {OffsetX} {OffsetY}
    - TileNum: Numbered 1 to 256, reading left to right, like a book.
    - OffsetX: This is the offset corresponding to the X-coordinate of the top left corner of the sprites tile number.
    - OffsetY: This is the offset corresponding to the Y-coordinate of the top left corner of the sprites tile number.
