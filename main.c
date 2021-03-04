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
#include "backtrack.c"

time_t t;

void clearMap(Map * map) {
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      map->tiles[(y * map->width) + x].unreachable = 0;
      map->tiles[(y * map->width) + x].connections = 0;
      map->tiles[(y * map->width) + x].distance = 0;
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

void applyMask(Map * map) {
  int mask[(map->width * map->height)];
  for (int i = 0; i < (map->width * map->height); i++) {
    mask[i] = 0;
  }

  mask[(10 * map->width) + 10] = 1;
  mask[(10 * map->width) + 11] = 1;
  mask[(11 * map->width) + 10] = 1;
  mask[(11 * map->width) + 11] = 1;

  int i;
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      i = ((y * map->width) + x);
      if (mask[i] == 1) {
        map->tiles[i].unreachable = 1;
        map->tiles[i].blocks |= COMPLETELY_BLOCKED;
        map->tiles[i - 1].blocks |= RIGHT;
        map->tiles[i - map->width].blocks |= DOWN;
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
  Tile * firstFurthest, *secondFurthest;
  srand((unsigned) time(&t));

  Map map;
  Event event;
  initRenderer();
  initMap(&map);
  applyMask(&map);
  sidewinder_map(&map);

  while (1) {
    renderPuzzle(&map);

    event = blockInput();
    if (event == QUIT) {
      return 0;
    } else if (event == REDO) {
      clearMap(&map);
      applyMask(&map);
      /* sidewinder_map(&map); */
      backtrack(&map);
      firstFurthest = distanceFrom(&map, 0,0);
      secondFurthest = distanceFrom(&map, firstFurthest->x, firstFurthest->y);
      printf("Longest path is from (%i,%i) -> (%i,%i)\n",
             secondFurthest->x, secondFurthest->y,
             firstFurthest->x, firstFurthest->y
             );
      printf("deadends: %i\n", countDeadends(&map));
    }
  }

  closeRenderer();
  return 0;
}
