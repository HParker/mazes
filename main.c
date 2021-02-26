// Some maze generation code inspired by the mazes for programmers book
#include <SDL.h>
#include <stdlib.h>
#include <SDL_opengl.h>

#include "util.c"

#include "b_tree.c"
#include "sidewinder.c"
#include "random_walk.c"
#include "wilson.c"
#include "hunt_and_kill.c"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_GLContext maincontext; /* Our opengl context handle */
SDL_Event e;
time_t t;

void renderTile(Tile * tile, int x, int y) {
  int x1 = x * TILE_SIZE;
  int y1 = y * TILE_SIZE;
  int x2 = (x+1) * TILE_SIZE;
  int y2 = (y+1) * TILE_SIZE;

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  if ((tile->connections & DOWN) == 0 || (tile->blocks & DOWN) != 0) {
    SDL_RenderDrawLine(renderer, x1, y2, x2, y2);
  }
  if ((tile->connections & RIGHT) == 0 || (tile->blocks & RIGHT) != 0) {
    SDL_RenderDrawLine(renderer, x2, y1, x2, y2);
  }
}

void renderPuzzle(Map * map) {
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      renderTile(&map->tiles[(y * map->width) + x], x, y);
    }
  }
  SDL_RenderPresent(renderer);
}

void startRender() {
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);
}

void endRender() {
  SDL_RenderPresent(renderer);
}

void renderMap(Map * map) {
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      renderTile(&map->tiles[(y * map->width) + x], x, y);
    }
  }
}

void debugRenderCursor(Map * map, int cursorX, int cursorY, int r, int b, int g) {
  SDL_Rect sdlRect;
  SDL_SetRenderDrawColor(renderer, r,g,b, 0xFF);
  sdlRect.x = ((cursorX * TILE_SIZE) + 1);
  sdlRect.y = ((cursorY * TILE_SIZE) + 1);
  sdlRect.w = TILE_SIZE - 1;
  sdlRect.h = TILE_SIZE - 1;
  SDL_RenderFillRect(renderer, &sdlRect);
}

void delay(int ms) {
  SDL_Delay(ms);
}

void debugRenderPuzzle(Map * map, int * visited, int cursorX, int cursorY) {
  SDL_Rect sdlRect;
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 160, 160, 160, 0xFF);
  sdlRect.x = (cursorX * TILE_SIZE);
  sdlRect.y = (cursorY * TILE_SIZE);
  sdlRect.w = TILE_SIZE;
  sdlRect.h = TILE_SIZE;
  SDL_RenderFillRect(renderer, &sdlRect);

  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      if (visited[(y  * map->width) + x]) {
        renderTile(&map->tiles[(y * map->width) + x], x, y);
      } else {
        SDL_SetRenderDrawColor(renderer, 224, 224, 224, 0xFF);
        sdlRect.x = (x * TILE_SIZE);
        sdlRect.y = (y * TILE_SIZE);
        sdlRect.w = TILE_SIZE;
        sdlRect.h = TILE_SIZE;
        SDL_RenderFillRect(renderer, &sdlRect);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

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
  map->width = MAP_SIZE;
  map->height = MAP_SIZE;
  map->tileCount = map->height * map->width;
  map->tiles = malloc(sizeof(Tile) * map->tileCount);
  clearMap(map);
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

int main(int argc, char ** argv) {
  srand((unsigned) time(&t));

  Map map;
  Event event;
  initRenderer();
  initMap(&map);
  sidewinder_map(&map);

  while (1) {
    renderPuzzle(&map);

    event = blockInput();
    if (event == QUIT) {
      return 0;
    } else if (event == REDO) {
      clearMap(&map);
      hunt_and_kill(&map);
    }
  }

  closeRenderer();
  return 0;
}
