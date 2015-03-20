#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

#include "Texture.h"
#include "Renderable.h"

class Sprite : public Renderable {
public:
  Sprite();
  Sprite(float x, float y, float width, float height, const ShaderProgram& program, bool isFixed);
  
  virtual ~Sprite();
  
  void Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View);
  void GenUVBuffers();
  void SetTexture(const Texture &texture, Rect textureRect);
  void SetTexture(const Texture &texture);

private:
  Rect textureRect;

  GLfloat uv_buffer[12];
  GLuint uvbuffer;
};