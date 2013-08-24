#pragma once

#include "Sprite.h"

#include <cstdlib>
#include <ctime>

/*
Level:
0 - Empty
1 - Block
2 - Player
3 - Guard
*/

class Level {
public:
  enum Type { EMPTY, BLOCK, GUARD, EXIT, ALL };
  
  Level(const Texture &texture) {
    sprites[BLOCK].SetTextureRect(Rect(0, 131, 32, 100), texture); 
    sprites[EMPTY].SetTextureRect(Rect(34, 131, 66, 100), texture);
    sprites[GUARD].SetTextureRect(Rect(102, 131, 134, 100), texture);
    sprites[EXIT].SetTextureRect(Rect(136, 131, 168, 100), texture);
    sprites[BLOCK].SetWidth(32);
    sprites[BLOCK].SetHeight(32);
    sprites[EMPTY].SetWidth(32);
    sprites[EMPTY].SetHeight(32);
    sprites[GUARD].SetWidth(32);
    sprites[GUARD].SetHeight(32);
    sprites[EXIT].SetWidth(32);
    sprites[EXIT].SetHeight(32);
  }

  void GenerateBlocks() {
    for (int i = 0; i < 9; i++) {
      int y = rand()%10+2;
      int x = rand()%15;
      level[y][x] = BLOCK;
      if (rand()%5!=0) {
        int count = rand()%3+1;
        for (int i = 0; i < count; i++) {
          int dest = rand()%4;
          switch (dest) {
          case 0:
            if (x-1 >= 0)
              level[y][x-1] = BLOCK;
            break;
          case 1:
            if (x+1 <= 15)
              level[y][x+1] = BLOCK;
            break;
          case 2:
            if (y-1 >= 0)
              level[y-1][x] = BLOCK;
            break;
          case 3:
            if (y+1 <= 15)
              level[y+1][x] = BLOCK;
            break;
          }
        }
      }
    }
  }

  bool CheckOnValid(int y, int x) {
    if (y+1<=15 && level[y+1][x] != EMPTY) return false;
    if (y-1>=0 && level[y-1][x] != BLOCK) return false;
    if (x-1>=0 && level[y][x-1] != EMPTY) return false;
    if (x+1<=15 && level[y][x+1] != EMPTY) return false;
    return true;
  }

  void GenerateGuard() {
    int x;
    int y;
    do {
      x = rand()%15;
      y = rand()%15;
    }
    while (level[y][x] != BLOCK);
    while (level[y+1][x] != EMPTY) y++;
    if (CheckOnValid(y, x)) {
      level[y][x] = GUARD;
    }
    else
      GenerateGuard();
  }
  
  void GenerateLevel() {
    srand(time(NULL));
    for (int i = 0; i < 16; i++)
      for (int j = 0; j < 16; j++)
        level[i][j] = EMPTY;
    GenerateBlocks();
    int count = rand()%4+4;
    for (int i = 0; i < count; i++)
      GenerateGuard();
    level[0][14] = EXIT;
  }

  void Draw(const Texture &texture) {
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        float x = j*32;
        float y = i*32;
        sprites[level[i][j]].SetX(x);
        sprites[level[i][j]].SetY(y);
        sprites[level[i][j]].Draw(texture);
      }
    }
  }

  Type level[16][16];
  Sprite sprites[ALL];
};