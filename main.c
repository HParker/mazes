// Some maze generation code inspired by the mazes for programmers book
#include <SDL.h>
#include <stdlib.h>

#include "maze.h"
#include "render.c"
#include "util.c"

#include "b_tree.c"
#include "sidewinder.c"
#include "random_walk.c"
#include "wilson.c"
#include "hunt_and_kill.c"

time_t t;

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

int main() {
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
