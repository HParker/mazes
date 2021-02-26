#include "maze.h"
#include <stdlib.h>

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
