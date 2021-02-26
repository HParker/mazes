#include "maze.h"
#include <stdlib.h>

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
