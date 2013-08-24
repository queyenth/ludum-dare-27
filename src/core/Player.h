#pragma once
#include "Sprite.h"
#include "Physics.h"

class Player {
public:
  enum State { STADE, JUMP, FALL, ALL };
  
  Player(const Texture &texture) {
    sprite.SetHeight(32);
    sprite.SetWidth(32);
    sprite.SetX(32*7);
    sprite.SetY(0);
    sprite.SetTextureRect(Rect(68, 131, 100, 100), texture);
    currentJumpVel = 0;
    currentState = STADE;
  }
  
  ~Player() {}

  bool CheckCollide(Level &level) {
    bool isCollide = false;
    Rect playerRect = Rect(sprite.GetX(), sprite.GetY(), sprite.GetX()+sprite.GetWidth(), sprite.GetY()+sprite.GetHeight());
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        if (level.level[i][j] == Level::BLOCK) {
          isCollide = Physics::CheckCollide(playerRect, Rect(j*32, (i+1)*32, (j+1)*32, i*32));
        }
        if (isCollide) {
          sprite.SetY(i*32);
          return true;
        }
      }
    }
    return false;
  }

  void Draw(const Texture &texture, Level &level) {
    if (currentState == JUMP) {
      sprite.Move(0.0f, currentJumpVel);
      if (currentJumpVel <= 0)
        currentState = FALL;
      currentJumpVel -= G;
    }
    else if (currentState == FALL) {
      sprite.Move(0.0f, currentJumpVel);
      if (sprite.GetY() >= 0)
        currentJumpVel -= G;
      else
        currentState = STADE;
      if (CheckCollide(level)) {
        currentState = STADE;
      }
    }
    sprite.Draw(texture);
  }

  void Jump() {
    if (currentState == STADE) {
      currentJumpVel = JUMPVEL;
      currentState = JUMP;
    }
  }

  Sprite sprite;
  int currentJumpVel;
  State currentState;

  static const int G = 2;
  static const int JUMPVEL = 30;
};