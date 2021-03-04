#include "maze.h"
#include <stdlib.h>

void random_walk(Map * map) {
   // I am lazy, we can probably know how many neighbors there will be ahead of time
  int visitedCount = 0;
  Tile * tile = randomTile(map);
  Tile * neighborTile = NULL;
  Direction dir;

  int visited[(map->width * map->height)];
  for (int i = 0; i < (map->width * map->height); i++) {
    visited[i] = 0;
  }

  int at = accessableTiles(map);
  while(visitedCount < at) {
    dir = randomNeighbor(map, tile, &neighborTile);

    if (visited[(neighborTile->y * map->width) + neighborTile->x] == 0) {
      if (dir == NORTH) {
        neighborTile->connections |= DOWN;
      } else if (dir == SOUTH) {
        tile->connections |= DOWN;
      } else if (dir == EAST) {
        tile->connections |= RIGHT;
      } else if (dir == WEST) {
        neighborTile->connections |= RIGHT;
      }

      visited[(neighborTile->y * map->width) + neighborTile->x] = 1;
      visitedCount++;
    }
    debugRenderPuzzle(map, visited, tile->x, tile->y);
    tile = neighborTile;
  }
}
