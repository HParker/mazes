#include <SDL.h>
#include <stdlib.h>

#include "maze.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_GLContext maincontext; /* Our opengl context handle */
SDL_Event e;

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


  debugRenderCursor(x, y, 255, 255, 255 - ((tile->distance) % 255));

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  if ((tile->connections & DOWN) == 0 || (tile->blocks & DOWN) != 0) {
    SDL_RenderDrawLine(renderer, x1, y2, x2, y2);
  }
  if ((tile->connections & RIGHT) == 0 || (tile->blocks & RIGHT) != 0) {
    SDL_RenderDrawLine(renderer, x2, y1, x2, y2);
  }
  if (tile->unreachable == 1) {
    debugRenderCursor(x, y, 200, 200, 255);
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

void debugRenderCursor(int cursorX, int cursorY, int r, int b, int g) {
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

  if (cursorX != -1 || cursorY != -1) {
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 0xFF);
    sdlRect.x = (cursorX * TILE_SIZE);
    sdlRect.y = (cursorY * TILE_SIZE);
    sdlRect.w = TILE_SIZE;
    sdlRect.h = TILE_SIZE;
    SDL_RenderFillRect(renderer, &sdlRect);
  }

  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      if (visited[(y  * map->width) + x] == 0) {
        SDL_SetRenderDrawColor(renderer, 224, 224, 224, 0xFF);
        sdlRect.x = (x * TILE_SIZE) + 1;
        sdlRect.y = (y * TILE_SIZE) + 1;
        sdlRect.w = TILE_SIZE - 1;
        sdlRect.h = TILE_SIZE - 1;
        SDL_RenderFillRect(renderer, &sdlRect);
      }
      renderTile(&map->tiles[(y * map->width) + x], x, y);
    }
  }
  SDL_RenderPresent(renderer);
}
