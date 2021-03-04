#include "maze.h"
#include <stdlib.h>

Tile * randomTile(Map * map) {
  int x = rand() % map->width;
  int y = rand() % map->height;

  return &map->tiles[(y * map->width) + x];
}

Direction randomNeighbor(Map * map, Tile * tile, Tile ** neighborTile) {
  int neighborCount = 0;
  Tile * neighborTiles[10];
  Direction neighborDir[10];
  int tileKey = ((tile->y - 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & UP) == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = NORTH;
    neighborCount++;
  }

  tileKey = ((tile->y + 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & DOWN) == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = SOUTH;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x - 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & LEFT) == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = WEST;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x + 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & RIGHT) == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = EAST;
    neighborCount++;
  }

  int randomNeighborIndex = rand() % neighborCount;
  *neighborTile = neighborTiles[randomNeighborIndex];
  return neighborDir[randomNeighborIndex];
}

int unvisitedNeighbor(Map * map, Tile * tile, int * visited, Tile ** neighborTile) {
  int tileKey = ((tile->y - 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & UP) == 0 && ((map->tiles[tileKey].connections & DOWN) != 0) && visited[tileKey] == 0) {
    *neighborTile = &map->tiles[tileKey];
    return 0;
  }

  tileKey = ((tile->y + 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & DOWN) == 0 && ((tile->connections & DOWN) != 0) && visited[tileKey] == 0) {
    *neighborTile = &map->tiles[tileKey];
    return 0;
  }

  tileKey = (tile->y * map->width) + tile->x - 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & LEFT) == 0 && ((map->tiles[tileKey].connections & RIGHT) != 0) && visited[tileKey] == 0) {
    *neighborTile = &map->tiles[tileKey];
    return 0;
  }

  tileKey = (tile->y * map->width) + tile->x + 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & RIGHT) == 0 && ((tile->connections & RIGHT) != 0) && visited[tileKey] == 0) {
    *neighborTile = &map->tiles[tileKey];
    return 0;
  }
  return 1;
}

Direction randomUnvisitedNeighbor(Map * map, Tile * tile, int * visited, Tile ** neighborTile) {
  int neighborCount = 0;
  Tile * neighborTiles[10];
  Direction neighborDir[10];

  int tileKey = ((tile->y - 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & UP) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = NORTH;
    neighborCount++;
  }

  tileKey = ((tile->y + 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & DOWN) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = SOUTH;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x - 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & LEFT) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = WEST;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x + 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & RIGHT) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = EAST;
    neighborCount++;
  }

  if (neighborCount == 0) {
    return NO_DIRECTION;
  }
  int randomNeighborIndex = rand() % neighborCount;
  *neighborTile = neighborTiles[randomNeighborIndex];
  return neighborDir[randomNeighborIndex];
}

Direction randomVisitedNeighbor(Map * map, Tile * tile, int * visited, Tile ** neighborTile) {
  int neighborCount = 0;
  Tile * neighborTiles[10];
  Direction neighborDir[10];

  int tileKey = ((tile->y - 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & UP) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = NORTH;
    neighborCount++;
  }

  tileKey = ((tile->y + 1) * map->width) + tile->x;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & DOWN) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = SOUTH;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x - 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & LEFT) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = WEST;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x + 1;
  if (map->tiles[tileKey].unreachable == 0 && (tile->blocks & RIGHT) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = EAST;
    neighborCount++;
  }

  if (neighborCount == 0) {
    return NONE;
  }
  int randomNeighborIndex = rand() % neighborCount;
  *neighborTile = neighborTiles[randomNeighborIndex];
  return neighborDir[randomNeighborIndex];
}

int countDeadends(Map * map) {
  int deadends = 0;
  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {
      if (map->tiles[(y * map->width) + x].connections == UP) {
        deadends++;
      } else if (map->tiles[(y * map->width) + x].connections == DOWN) {
        deadends++;
      } else if (map->tiles[(y * map->width) + x].connections == LEFT) {
        deadends++;
      } else if (map->tiles[(y * map->width) + x].connections == RIGHT) {
        deadends++;
      }
    }
  }
  return deadends;
}

int accessableTiles(Map * map) {
  int accessableTiles = 0;
  for (int i = 0; i < (map->width * map->height); i++) {
    if (map->tiles[i].unreachable == 0) {
      accessableTiles++;
    }
  }
  return accessableTiles;
}

// Path finding
Tile * distanceFrom(Map * map, int x, int y) {
  Tile * lastTouchedTile;
  int distance = 0;
  int visitedCount = 0;
  int visited[(map->width * map->height)];
  for (int i = 0; i < (map->width * map->height); i++) {
    visited[i] = 0;
  }

  Tile * edge[map->width * map->height];

  int tempEdgeWidth = 0;
  Tile * tempEdge[map->width * map->height];


  int edgeWidth = 1;
  edge[0] = &map->tiles[(y * map->width) + x];
  edge[0]->distance = distance;
  visited[(edge[0]->y * map->width) + edge[0]->x] = 1;
  visitedCount++;
  distance++;

  int at = accessableTiles(map);
  while(visitedCount < at) {
    tempEdgeWidth = 0;

    // refill
    for (int i = 0; i < edgeWidth; i++) {
      int hasFirstNeighbor = unvisitedNeighbor(map, edge[i], visited, &tempEdge[tempEdgeWidth]);

      while (hasFirstNeighbor == 0) {
        lastTouchedTile = tempEdge[tempEdgeWidth];
        tempEdge[tempEdgeWidth]->distance = distance;
        visited[(tempEdge[tempEdgeWidth]->y * map->width) + tempEdge[tempEdgeWidth]->x] = 1;
        visitedCount++;
        tempEdgeWidth++;
        hasFirstNeighbor = unvisitedNeighbor(map, edge[i], visited, &tempEdge[tempEdgeWidth]);
      }
    }

    // copy over
    edgeWidth = tempEdgeWidth;
    for (int i = 0; i < tempEdgeWidth; i++) {
      edge[i] = tempEdge[i];
    }
    distance++;
    //draw
    debugRenderPuzzle(map, visited, -1, -1);
    delay(100);
  }
  return lastTouchedTile;
}
