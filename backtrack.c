#include "maze.h"
#include <stdlib.h>

void backtrack(Map * map) {
  int visitedCount = 0;
  int visited[(map->width * map->height)];
  for (int i = 0; i < (map->width * map->height); i++) {
    visited[i] = 0;
  }

  Tile * tile = randomTile(map);
  Tile * neighborTile;

  int pathIndex = 0;
  Tile * path[map->width * map->height];
  Direction pathDir[map->width * map->height];

  path[pathIndex] = tile;
  visited[(path[pathIndex]->y * map->width) + path[pathIndex]->x] = 1;
  visitedCount++;
  pathIndex++;

  while (visitedCount < (map->width * map->height)) {
    pathDir[pathIndex] = randomUnvisitedNeighbor(map, tile, visited, &path[pathIndex]);

    if (pathDir[pathIndex] == NO_DIRECTION) {
      // backtrack
      while (pathDir[pathIndex] == NO_DIRECTION && pathIndex > 0) {
        pathIndex--;
        tile = path[pathIndex];
        pathDir[pathIndex] = randomUnvisitedNeighbor(map, tile, visited, &path[pathIndex + 1]);
      }
      pathIndex++;

      neighborTile = path[pathIndex];
      visited[(neighborTile->y * map->width) + neighborTile->x] = 1;
      visitedCount++;

      // TODO: this sucks, since we backtracked, we assign the wrong direction before here
      Direction dir = pathDir[pathIndex - 1];
      if (dir == NORTH) {
        neighborTile->connections |= DOWN;
      } else if (dir == SOUTH) {
        tile->connections |= DOWN;
      } else if (dir == EAST) {
        tile->connections |= RIGHT;
      } else if (dir == WEST) {
        neighborTile->connections |= RIGHT;
      }

      tile = path[pathIndex];
      pathIndex++;
    } else {
      // render path
      startRender();
      renderMap(map);
      debugRenderCursor(path[pathIndex]->x, path[pathIndex]->y, 0, 100, 0);
      for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
          if (visited[(y * map->width) + x] == 1) {
            debugRenderCursor(x, y, 222, 222, 222);
          }
        }
      }
      endRender();
      delay(1);

      visited[(path[pathIndex]->y * map->width) + path[pathIndex]->x] = 1;
      visitedCount++;

      tile = path[pathIndex - 1];
      neighborTile = path[pathIndex];
      // Direction is on the neighbor, 0 index never has as direction
      Direction dir = pathDir[pathIndex];
      if (dir == NORTH) {
        neighborTile->connections |= DOWN;
      } else if (dir == SOUTH) {
        tile->connections |= DOWN;
      } else if (dir == EAST) {
        tile->connections |= RIGHT;
      } else if (dir == WEST) {
        neighborTile->connections |= RIGHT;
      }
      tile = path[pathIndex];
      pathIndex++;
    }
  }
}
