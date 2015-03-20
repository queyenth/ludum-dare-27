#pragma once

#include "Animation.h"
#include "Platokdef.h"

#include <map>

enum EntityType {
  GROUND,
  PLAYER,
  ENEMY
};

enum AnimationType {
  IDLE,
  MOVE,
  JUMP
};

class Entity {
public:
  Entity() {
    currentAnimation = nullptr;
  }
  Entity(EntityType type) {
    this->type = type;
    currentAnimation = nullptr;
  }
  Entity(EntityType etype, AnimationType atype, float x, float y, float width, float height, const ShaderProgram& program, bool isFixed) {
    animations.insert(std::pair<AnimationType, Animation *>(atype, new Animation(x, y, width, height, program, isFixed)));
    currentAnimation = animations[atype];
    currentAnimationType = atype;
    type = etype;
  }
  virtual ~Entity() {
    for (auto i : animations)
      delete i.second;
  }

  Animation *GetCurrentAnimation() {
    return currentAnimation;
  }

  AnimationType GetCurrentAnimationType() {
    return currentAnimationType;
  }

  Animation *AddAnimation(AnimationType type, float x, float y, float width, float height, const ShaderProgram& program, bool isFixed) {
    animations.insert(std::pair<AnimationType, Animation *>(type, new Animation(x, y, width, height, program, isFixed)));
    currentAnimation = animations[type];
    currentAnimationType = type;
    return currentAnimation;
  }

  Animation *SetAnimation(AnimationType type) {
    if (currentAnimation != nullptr)
      animations[type]->CopyLocation(currentAnimation);
    currentAnimation = animations[type];
    currentAnimationType = type;
    return currentAnimation;
    /*for (auto i : animations) {
      if (i.first == type) {
        if (currentAnimation != nullptr)
          i.second->CopyLocation(currentAnimation);
        currentAnimation = i.second;
        currentAnimationType = type;
        return currentAnimation;
      }
    }*/
  }

  Animation *GetAnimation(AnimationType type) {
    return animations[type];
    /*for (auto i : animations)
      if (i.first == type)
        return i.second;*/
  }

  void SetEntityType(EntityType type) {
    this->type = type;
  }

  inline void Draw(const Texture &texture, const glm::mat4 &projection, const glm::mat4 &view) {
    currentAnimation->Draw(texture, projection, view);
  }

  inline operator EntityType() {
    return type;
  }

  inline EntityType GetEntityType() {
    return type;
  }

protected:
  std::map<AnimationType, Animation *> animations;
  AnimationType currentAnimationType;
  Animation *currentAnimation;
  EntityType type;
};

