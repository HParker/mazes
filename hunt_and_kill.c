#include "maze.h"
#include <stdlib.h>

void hunt_and_kill(Map * map) {
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
      // hunt
      pathIndex = 0;
      int found = 0;
      for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
          if (found == 0 && visited[(y * map->width) + x] == 0) {
            startRender();
            // render path
            renderMap(map);

            debugRenderCursor(map, x, y, 222, 222, 222);

            endRender();
            delay(100);

            pathDir[pathIndex] = randomVisitedNeighbor(map, &map->tiles[(y * map->width) + x], visited, &path[pathIndex]);
            if (pathDir[pathIndex] == NO_DIRECTION) {
              // continue..
            } else {
              // connect and switch modes

              tile = &map->tiles[(y * map->width) + x];
              visited[(y * map->width) + x] = 1;
              visitedCount++;

              neighborTile = path[pathIndex];
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
              found = 1;
            }
          }
        }
      }
      tile = path[pathIndex];
      pathIndex++;
    } else {

      startRender();
      // render path
      renderMap(map);

      debugRenderCursor(map, path[pathIndex]->x, path[pathIndex]->y, 0, 100, 0);

      for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
          if (visited[(y * map->width) + x] == 1) {
            debugRenderCursor(map, x, y, 222, 222, 222);
          }
        }
      }

      endRender();
      delay(100);

      // kill
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
