#pragma once
#include "Sprite.h"
#include "Physics.h"

class Player {
public:
  enum State { STADE, JUMP, FALL, MOVE, ALL };
  
  Player(const Texture &texture) {
    sprite.SetHeight(64);
    sprite.SetWidth(32);
    sprite.SetX(32*7);
    sprite.SetY(0);
    sprite.SetTextureRect(Rect(0, 199, 32, 133), texture);
    move.SetHeight(64);
    move.SetWidth(32);
    move.SetSpeed(100);
    int offsetY = 199;
    int height = 66;
    int offsetX = 66;
    int width = 32;
    move.AddFrame(Rect(offsetX, offsetY, offsetX+width, offsetY-height), texture);
    move.AddFrame(Rect(offsetX+width, offsetY, offsetX*2, offsetY-height), texture);
    move.AddFrame(Rect(offsetX, offsetY, offsetX+width, offsetY-height), texture);
    move.AddFrame(Rect(offsetX*2, offsetY, offsetX*2+width, offsetY-height), texture);
    currentJumpVel = 0;
    currentState = STADE;

    experience = 0;
  }
  
  ~Player() {}

  bool CheckCollide(Level &level, Level::Type type) {
    bool isCollide = false;
    Rect playerRect = Rect(sprite.GetX(), sprite.GetY(), sprite.GetX()+sprite.GetWidth(), sprite.GetY()+sprite.GetHeight());
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        if (level.level[i][j] == type) {
          isCollide = Physics::CheckCollide(playerRect, Rect(j*32, i*32, (j+1)*32, (i+1)*32));
        }
        if (isCollide)
          return true;
      }
    }
    return false;
  }

  void Update(Level &level) {
    // Gravity and jump
    if (currentState == JUMP) {
      sprite.Move(0.0f, currentJumpVel);
      if (currentJumpVel <= 0)
        currentState = FALL;
      currentJumpVel -= G;
      if (CheckCollide(level, Level::BLOCK)) {
        currentState = FALL;
        while (CheckCollide(level, Level::BLOCK)) sprite.Move(0.0f, -1.0f);
        currentJumpVel = 0;
      }
    }
    else if (currentState == FALL) {
      sprite.Move(0.0f, currentJumpVel);
      if (sprite.GetY() >= 0 && !CheckCollide(level, Level::BLOCK))
        currentJumpVel -= G;
      else {
        while (sprite.GetY() < 0 || CheckCollide(level, Level::BLOCK)) sprite.Move(0.0f, 1.0f);
        currentState = STADE;
        currentJumpVel = 0;
      }
    }
    else if (!CheckCollide(level, Level::BLOCK) && sprite.GetY() > 0) {
      currentState = FALL;
    }

    Rect playerRect = Rect(sprite.GetX(), sprite.GetY(), sprite.GetX()+sprite.GetWidth(), sprite.GetY()+sprite.GetHeight());
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        if (level.level[i][j] == Level::GUARD) {
          if (Physics::CheckCollide(playerRect, Rect(j*32, i*32, (j+1)*32, (i+1)*32))) {
            level.level[i][j] = Level::EMPTY;
            level.guards--;
            PlaySound(L"snd\\hit.wav", NULL, SND_ASYNC);
            experience+=2;
          }
        }
      }
    }
  }

  void Draw(const Texture &texture) {
    if (currentState == STADE) {
      sprite.SetTextureRect(Rect(0, 199, 32, 133), texture);
      move.SetCurrentFrame(0);
    }
    else if (currentState == MOVE) {
      move.SetX(sprite.GetX());
      move.SetY(sprite.GetY());
      move.Draw(texture);
      return ;
    }
    sprite.Draw(texture);
  }

  void Jump(const Texture &texture) {
    if (currentState == STADE) {
      currentJumpVel = JUMPVEL;
      currentState = JUMP;
      PlaySound(L"snd\\jump.wav", NULL, SND_ASYNC);
      sprite.SetTextureRect(Rect(33, 199, 65, 133), texture);
    }
  }

  Sprite sprite;
  Animation move;
  int currentJumpVel;
  State currentState;
  int experience;

  static const int G = 2;
  static const int JUMPVEL = 30;
};