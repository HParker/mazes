#include "maze.h"
#include <stdlib.h>

void wilson(Map * map) {
  int done = 1;
  int visitedCount = 0;
  int pathIndex = 0;
  Tile * path[map->width * map->height];
  int pathDir[map->width * map->height];
  int visited[(map->width * map->height)];
  for (int i = 0; i < (map->width * map->height); i++) {
    visited[i] = 0;
  }
  Tile * firstVisit;
  Tile * tile;
  Tile * neighborTile;
  firstVisit = randomTile(map);
  visited[(firstVisit->y * map->width) + firstVisit->x] = 1;
  visitedCount++;

  while (visitedCount < (map->width * map->height)) {
    if (done) {
      pathIndex = 0;
      path[pathIndex] = randomTile(map);
      while (visited[(path[pathIndex]->y * map->width) + path[pathIndex]->x]) {
        path[pathIndex] = randomTile(map);
      }
      done = 0;
    }
    tile = path[pathIndex];
    pathIndex++;
    pathDir[pathIndex] = randomNeighbor(map, tile, &path[pathIndex]);

    startRender();
    // render path
    renderMap(map);

    debugRenderCursor(map, firstVisit->x, firstVisit->y, 100,100,100);

    for (int i = 0; i <= pathIndex; i++) {
      debugRenderCursor(map, path[i]->x, path[i]->y,50,100,20);
    }

    endRender();
    delay(10);

    for (int i = pathIndex-1; i >= 0; i--) {
      if (path[i]->x == path[pathIndex]->x && path[i]->y == path[pathIndex]->y) {
        pathIndex = i;
      }
    }

    if (visited[(path[pathIndex]->y * map->width) + path[pathIndex]->x]) {
      done = 1;

      // Draw path
      for (int i = 0; i < pathIndex; i++) {
        visited[(path[i]->y * map->width) + path[i]->x] = 1;
        visitedCount++;
        tile = path[i];
        neighborTile = path[i + 1];
        // Direction is on the neighbor, 0 index never has as direction
        Direction dir = pathDir[i + 1];
        if (dir == NORTH) {
          neighborTile->connections |= DOWN;
        } else if (dir == SOUTH) {
          tile->connections |= DOWN;
        } else if (dir == EAST) {
          tile->connections |= RIGHT;
        } else if (dir == WEST) {
          neighborTile->connections |= RIGHT;
        }
      }
    }
  }
}
