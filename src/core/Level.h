#pragma once

#include "Sprite.h"

#include <cstdlib>
#include <ctime>

class Level {
public:
  enum Type { EMPTY, BLOCK, GUARD, EXIT, ALL };
  
  Level(const Texture &texture, const ShaderProgram &program) {
    sprites[BLOCK].SetTextureRect(Rect(0, 132, 32, 100), texture);
    sprites[GUARD].SetTextureRect(Rect(99, 132, 131, 100), texture);
    sprites[EXIT].SetTextureRect(Rect(132, 265, 164, 200), texture);

    for (int i = EMPTY; i < ALL; i++) {
      if (i != EXIT) {
        sprites[i].SetHeight(64);
      }
      sprites[i].SetWidth(32);
      sprites[i].SetHeight(32);
      sprites[i].GenVertexBuffers();
      sprites[i].GenUVBuffers();
      sprites[i].SetShaderProgram(program);
    }

    srand(time(NULL));
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
    bool isGenerated = false;
    while (!isGenerated) {
      do {
        x = rand()%15;
        y = rand()%15;
      }
      while (level[y][x] != BLOCK);
      while (level[y+1][x] != EMPTY) y++;
      if (CheckOnValid(y, x)) {
        level[y][x] = GUARD;
        isGenerated = true;
      }
    }
  }
  
  void GenerateLevel() {
    for (int i = 0; i < 16; i++)
      for (int j = 0; j < 16; j++)
        level[i][j] = EMPTY;
    GenerateBlocks();
    int count = rand()%4+4;
    for (int i = 0; i < count; i++)
      GenerateGuard();
    level[0][7] = EXIT;
    guards = 0;
    for (int i = 0; i < 16; i++)
      for (int j = 0; j < 16; j++)
        if (level[i][j] == Level::GUARD)
          guards++;
  }

  void Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View) {
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        if (level[i][j] == EMPTY) continue;
        float x = j*32;
        float y = i*32;
        float width = sprites[level[i][j]].GetWidth();
        float height = sprites[level[i][j]].GetHeight();
        sprites[level[i][j]].SetX(x+width/2);
        sprites[level[i][j]].SetY(y+height/2);
        sprites[level[i][j]].Draw(texture, Projection, View);
      }
    }
    sprites[level[0][7]].Draw(texture, Projection, View);
  }

  Type level[16][16];
  Sprite sprites[ALL];
  int guards;
};