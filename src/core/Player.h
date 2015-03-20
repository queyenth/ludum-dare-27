#pragma once

#include "Entity.h"

#include "Level.h"

class Player : public Entity {
public:
  enum State {
    STADE,
    MOVE,
    JUMP,
    FALL,
    ALL
  };

  Player() {
  }
  Player(float x, float y, const Texture &texture, const ShaderProgram& program) {
    animations.insert(std::pair<AnimationType, Animation*>(AnimationType::IDLE, new Animation(x, y, 32, 64, program, false)));
    Animation *animation = GetAnimation(AnimationType::IDLE);
    animation->AddFrame(texture, Rect(0, 133, 32, 197));
    animation->SetLoopMode(true);
    animation->GenVertexBuffers();
    animation->GenUVBuffers();
    animations.insert(std::pair<AnimationType, Animation*>(AnimationType::MOVE, new Animation(x, y, 32, 64, program, false)));
    animation = GetAnimation(AnimationType::MOVE);
    animation->AddFrame(texture, Rect(66, 133, 97, 197));
    animation->AddFrame(texture, Rect(99, 133, 130, 197));
    animation->AddFrame(texture, Rect(132, 133, 163, 197));
    animation->SetLoopMode(true);
    animation->GenVertexBuffers();
    animation->GenUVBuffers();
    animations.insert(std::pair<AnimationType, Animation*>(AnimationType::JUMP, new Animation(x, y, 32, 64, program, false)));
    animation = GetAnimation(AnimationType::JUMP);
    animation->AddFrame(texture, Rect(33, 133, 64, 197));
    animation->SetLoopMode(true);
    animation->GenVertexBuffers();
    animation->GenUVBuffers();
    SetAnimation(AnimationType::IDLE);
    currentJumpVel = 0;
    currentState = STADE;
  }

  virtual ~Player() {

  }

  bool CheckCollide(Level &level, EntityType type) {
    bool isCollide = false;
    Animation *animation = GetCurrentAnimation();
    Rect playerRect = animation->GetRect();
    for (auto i : level.entities) {
      if (*i == type) {
        Rect blockRect = i->GetCurrentAnimation()->GetRect();
        isCollide = Physics::CheckCollide(playerRect, blockRect);
      }
      if (isCollide)
        return true;
    }
    return false;
  }

  void Update(Level &level) {
    Animation *animation = GetCurrentAnimation();

    if (currentState == JUMP) {
      GetCurrentAnimation()->Move(0.0f, currentJumpVel);
      if (currentJumpVel >= 0)
        currentState = FALL;
      currentJumpVel -= G;
      if (CheckCollide(level, EntityType::GROUND)) {
        currentState = FALL;
        while (CheckCollide(level, EntityType::GROUND)) animation->Move(0.0f, 1.0f);
        currentJumpVel = 0;
      }
    }
    else if (currentState == FALL) {
      animation->Move(0.0f, currentJumpVel);
      if (animation->GetY() < HEIGHT-64 && !CheckCollide(level, EntityType::GROUND))
        currentJumpVel -= G;
      else {
        while (animation->GetY() >= HEIGHT-64 || CheckCollide(level, EntityType::GROUND)) animation->Move(0.0f, -1.0f);
        currentState = STADE;
        SetAnimation(AnimationType::IDLE);
        currentJumpVel = 0;
      }
    }
    else if (currentState == STADE && !CheckCollide(level, EntityType::GROUND) && animation->GetY() <= HEIGHT-64) {
      currentState = FALL;
    }
  }

  void Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View) {
    Entity::Draw(texture, Projection, View);
  }

  void Jump() {
    if (currentState == STADE || currentState == MOVE) {
      currentJumpVel = JUMPVEL;
      currentState = JUMP;
      SetAnimation(AnimationType::JUMP);
    }
  }

  void SetState(State newState) {
    if (currentState == JUMP || currentState == FALL || currentState == newState) {
    }
    else {
      currentState = newState;
      SetAnimation(AnimationType::MOVE);
    }
  }

  State currentState;
  int currentJumpVel;
  
  static const int G = -2;
  static const int JUMPVEL = -30;
};
