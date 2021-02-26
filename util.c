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

  if ((tile->blocks & UP) == 0) {
    neighborTiles[neighborCount] = &map->tiles[((tile->y - 1) * map->width) + tile->x];
    neighborDir[neighborCount] = NORTH;
    neighborCount++;
  }
  if ((tile->blocks & DOWN) == 0) {
    neighborTiles[neighborCount] = &map->tiles[((tile->y + 1) * map->width) + tile->x];
    neighborDir[neighborCount] = SOUTH;
    neighborCount++;
  }

  if ((tile->blocks & LEFT) == 0) {
    neighborTiles[neighborCount] = &map->tiles[(tile->y * map->width) + tile->x - 1];
    neighborDir[neighborCount] = WEST;
    neighborCount++;
  }

  if ((tile->blocks & RIGHT) == 0) {
    neighborTiles[neighborCount] = &map->tiles[(tile->y * map->width) + tile->x + 1];
    neighborDir[neighborCount] = EAST;
    neighborCount++;
  }

  int randomNeighborIndex = rand() % neighborCount;
  *neighborTile = neighborTiles[randomNeighborIndex];
  return neighborDir[randomNeighborIndex];
}

Direction randomUnvisitedNeighbor(Map * map, Tile * tile, int * visited, Tile ** neighborTile) {
  int neighborCount = 0;
  Tile * neighborTiles[10];
  Direction neighborDir[10];

  int tileKey = ((tile->y - 1) * map->width) + tile->x;
  if ((tile->blocks & UP) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = NORTH;
    neighborCount++;
  }

  tileKey = ((tile->y + 1) * map->width) + tile->x;
  if ((tile->blocks & DOWN) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = SOUTH;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x - 1;
  if ((tile->blocks & LEFT) == 0 && visited[tileKey] == 0) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = WEST;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x + 1;
  if ((tile->blocks & RIGHT) == 0 && visited[tileKey] == 0) {
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
  if ((tile->blocks & UP) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = NORTH;
    neighborCount++;
  }

  tileKey = ((tile->y + 1) * map->width) + tile->x;
  if ((tile->blocks & DOWN) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = SOUTH;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x - 1;
  if ((tile->blocks & LEFT) == 0 && visited[tileKey] == 1) {
    neighborTiles[neighborCount] = &map->tiles[tileKey];
    neighborDir[neighborCount] = WEST;
    neighborCount++;
  }

  tileKey = (tile->y * map->width) + tile->x + 1;
  if ((tile->blocks & RIGHT) == 0 && visited[tileKey] == 1) {
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
