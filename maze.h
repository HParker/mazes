#ifndef MAZE
#define MAZE

#define WINDOW_SIZE 840
#define TILE_SIZE 20
#define MAP_SIZE 10

// 0 0 0 0 1 1 1 1
#define COMPLETELY_BLOCKED 15

// 0 0 0 0 0 0 0 1
#define UP 1
// 0 0 0 0 0 0 1 0
#define LEFT 2
// 0 0 0 0 0 1 0 0
#define DOWN 4
// 0 0 0 0 1 0 0 0
#define RIGHT 8
// 0 0 0 0 0 0 0 0
#define NONE 0

typedef struct Tile {
  int x;
  int y;
  char blocks;
  char connections;
  // 0 0 0 0 0 0 0 0
  // ^ ^ ^ ^ ^ ^ ^ ^
  // | | | | | | | -> connected up
  // | | | | | | ---> connected left
  // | | | | | -----> connected down
  // | | | | -------> connected right
  // | | | ---------> unused
  // | | -----------> unused
  // | -------------> unused
  // ---------------> unused
} Tile;

typedef struct Map {
  unsigned int tileCount;
  int width;
  int height;
  Tile * tiles;
} Map;

typedef enum Event
  {
   NOTHING = 0,
   QUIT = 1,
   REDO = 2
  } Event;

typedef enum Direction
  {
   NORTH = 0,
   WEST = 1,
   SOUTH = 3,
   EAST = 4,
   NO_DIRECTION = 5
  } Direction;



extern void debugRenderPuzzle(Map * map, int * visited, int x, int y);

extern void debugRenderCursor(int cursorX, int cursorY, int r, int b, int g);
extern void startRender();
extern void endRender();
extern void renderMap(Map * map);
extern void delay(int ms);

extern Tile * randomTile(Map * map);
extern Tile * randomUnvisitedTile(Map * map);
extern Direction randomNeighbor(Map * map, Tile * tile, Tile ** neighborTile);
extern Direction randomUnvisitedNeighbor(Map * map, Tile * tile, int * visited, Tile ** neighborTile);
extern Direction randomVisitedNeighbor(Map * map, Tile * tile, int * visited, Tile ** neighborTile);

#endif
