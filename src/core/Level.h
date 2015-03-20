#pragma once

#include "Entity.h"

class Level {
public:
  Level() {
  }
  virtual ~Level() {
    for (auto i : entities)
      delete i;
  }

  void GenLevel(const Texture &texture, const ShaderProgram &program) {
    for (int i = 0; i < 32; i++) {
      entities.push_back(new Entity(EntityType::GROUND, AnimationType::IDLE, i*32.0f, HEIGHT - 32.0f, 32.0f, 32.0f, program, false));
      entities[i]->GetCurrentAnimation()->AddFrame(texture, Rect(0, 101, 32, 132));
      entities[i]->GetCurrentAnimation()->GenVertexBuffers();
      entities[i]->GetCurrentAnimation()->GenUVBuffers();
    }
  }

  inline void Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View) {
    for (unsigned int i = 0; i < entities.size(); i++) {
      GLuint ModelMatrixID = glGetUniformLocation(entities[i]->GetCurrentAnimation()->GetShaderProgram(), "M");
      glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &entities[i]->GetCurrentAnimation()->GetModelMatrix()[0][0]);
      entities[i]->Draw(texture, Projection, View);
    }
  }

  std::vector<Entity *> entities;
};
