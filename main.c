// Some maze generation code inspired by the mazes for programmers book
#include <SDL.h>
#include <stdlib.h>
#include <SDL_opengl.h>

#define WINDOW_SIZE 840
#define TILE_SIZE 40
SDL_Window * window;
SDL_Renderer * renderer;
SDL_GLContext maincontext; /* Our opengl context handle */
SDL_Event e;
time_t t;

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
   EAST = 4
  } Direction;

void initRenderer() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "ERROR Initializing SDL2\n");
  }

  window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "ERROR making window\n");
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "ERROR making renderer\n");
  }
}

void closeRenderer() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void clearMap(Map * map) {
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      map->tiles[(y * map->width) + x].connections = 0;
      map->tiles[(y * map->width) + x].blocks = 0;
      if (x == 0) {
        map->tiles[(y * map->width) + x].blocks |= LEFT;
      }
      if (y == 0) {
        map->tiles[(y * map->width) + x].blocks |= UP;
      }
      if (x + 1 == map->width) {
        map->tiles[(y * map->width) + x].blocks |= RIGHT;
      }
      if (y + 1 == map->height) {
        map->tiles[(y * map->width) + x].blocks |= DOWN;
      }
    }
  }
}

void initMap(Map * map) {
  map->width = 10;
  map->height = 10;
  map->tileCount = map->height * map->width;
  map->tiles = malloc(sizeof(Tile) * map->tileCount);
  clearMap(map);
}

void sidewinder_map(Map * map) {
  int tilesInRun = 0;

  Tile * tile;
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      tile = &map->tiles[(y * map->width) + x];
      tile->x = x;
      tile->y = y;
      int shouldClose = 0;
      if (x + 1 == map->width || (y + 1 != map->height && rand() % 2 == 0)) {
        shouldClose = 1;
      }


      if (shouldClose) {
        int randomTileInRun;
        if (tilesInRun == 0) {
          randomTileInRun = 0;
        } else {
          randomTileInRun = (rand() % tilesInRun);
        }
        map->tiles[((y * map->width) + x) - randomTileInRun].connections |= DOWN;
        tilesInRun = 0;
      } else {
        tile->connections = RIGHT;
        tilesInRun++;
      }
    }
  }
}


void b_tree_map(Map * map) {
  Tile * tile;
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      tile = &map->tiles[(y * map->width) + x];

      if ((tile->blocks & DOWN) != 0 && (tile->blocks & RIGHT) != 0) {
        tile->connections = 0;
      } else if ((tile->blocks & DOWN) != 0) {
        tile->connections = RIGHT;
      } else if ((tile->blocks & RIGHT) != 0) {
        tile->connections = DOWN;
      } else if (rand() % 2 == 0) {
        tile->connections = DOWN;
      } else {
        tile->connections = RIGHT;
      }
    }
  }
}

int neighbors(Map * map, Tile ** neighbors, int x, int y) {
  int neighborCount = 0;
  Tile * tile = &map->tiles[(y * map->width) + x];
  if ((tile->blocks & UP) == 0) {
    neighbors[neighborCount] = &map->tiles[((y - 1) * map->width) + x];
    neighborCount++;
  }
  if ((tile->blocks & DOWN) == 0) {
    neighbors[neighborCount] = &map->tiles[((y + 1) * map->width) + x];
    neighborCount++;
  }

  if ((tile->blocks & LEFT) == 0) {
    neighbors[neighborCount] = &map->tiles[(y * map->width) + x - 1];
    neighborCount++;
  }

  if ((tile->blocks & RIGHT) == 0) {
    neighbors[neighborCount] = &map->tiles[(y * map->width) + x + 1];
    neighborCount++;
  }
  return neighborCount;
}

void random_walk(Map * map) {
  Tile * tile;
  int neighborCount = 0;
  int neighborX[10]; // I am lazy, we can probably know how many neighbors there will be ahead of time
  int neighborY[10]; // I am lazy, we can probably know how many neighbors there will be ahead of time
  Direction neighborDir[10]; // I am lazy, we can probably know how many neighbors there will be ahead of time

  int visitedCount = 0;
  int x = rand() % map->width;
  int y = rand() % map->height;

  int visited[(map->width * map->height)];
  for (int i = 0; i < (map->width * map->height); i++) {
    visited[i] = 0;
  }

  while (visitedCount < (map->width * map->height)) {
    tile = &map->tiles[(y * map->width) + x];
    int neighborCount = 0;
    Tile * tile = &map->tiles[(y * map->width) + x];

    if ((tile->blocks & UP) == 0) {
      neighborX[neighborCount] = x;
      neighborY[neighborCount] = y - 1;
      neighborDir[neighborCount] = NORTH;
      neighborCount++;
    }
    if ((tile->blocks & DOWN) == 0) {
      neighborX[neighborCount] = x;
      neighborY[neighborCount] = y + 1;
      neighborDir[neighborCount] = SOUTH;
      neighborCount++;
    }

    if ((tile->blocks & LEFT) == 0) {
      neighborX[neighborCount] = x - 1;
      neighborY[neighborCount] = y;
      neighborDir[neighborCount] = WEST;
      neighborCount++;
    }

    if ((tile->blocks & RIGHT) == 0) {
      neighborX[neighborCount] = x + 1;
      neighborY[neighborCount] = y;
      neighborDir[neighborCount] = EAST;
      neighborCount++;
    }
    int randomNeighborInt = rand() % neighborCount;
    x = neighborX[randomNeighborInt];
    y = neighborY[randomNeighborInt];
    Direction dir = neighborDir[randomNeighborInt];

    if (visited[(y * map->width) + x] == 0) {
      if (dir == NORTH) {
        map->tiles[(y * map->width) + x].connections |= DOWN;
      } else if (dir == SOUTH) {
        tile->connections |= DOWN;
      } else if (dir == EAST) {
        tile->connections |= RIGHT;
      } else if (dir == WEST) {
        map->tiles[(y * map->width) + x].connections |= RIGHT;
      }

      visited[(y * map->width) + x] = 1;
      visitedCount++;
    }
  }
}

Event blockInput() {
  // block on input for now. I don't think we have to do this forever.
  while (1) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        return QUIT;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_r:
          return REDO;
        }
      }
    }
  }
  return NOTHING;
}

void renderTile(Tile * tile, int x, int y) {
  int x1 = x * TILE_SIZE;
  int y1 = y * TILE_SIZE;
  int x2 = (x+1) * TILE_SIZE;
  int y2 = (y+1) * TILE_SIZE;

  /* if ((tile->connections & DOWN) == 0 && (tile->connections & RIGHT) == 0) { */
  /*   return; */
  /*   // NOTHING */
  /* } */

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  if ((tile->connections & DOWN) == 0 || (tile->blocks & DOWN) != 0) {
    SDL_RenderDrawLine(renderer, x1, y2, x2, y2);
  }
  if ((tile->connections & RIGHT) == 0 || (tile->blocks & RIGHT) != 0) {
    SDL_RenderDrawLine(renderer, x2, y1, x2, y2);
  }
}

void renderPuzzle(Map * map) {
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      renderTile(&map->tiles[(y * map->width) + x], x, y);
    }
  }
}

int main(int argc, char ** argv) {
  srand((unsigned) time(&t));

  Map map;
  Event event;
  initRenderer();
  initMap(&map);
  /* b_tree_map(&map); */
  /* sidewinder_map(&map); */
  random_walk(&map);

  while (1) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    renderPuzzle(&map);
    SDL_RenderPresent(renderer);

    event = blockInput();
    if (event == QUIT) {
      return 0;
    } else if (event == REDO) {
      clearMap(&map);
      random_walk(&map);
    }

  }


  closeRenderer();
  return 0;
}
