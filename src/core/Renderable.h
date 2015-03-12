#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

#include "Rect.h"
#include "Texture.h"
#include "Color.h"
#include "ShaderProgram.h"
#include "Shader.h"

class Renderable {
public:
  Renderable();
  Renderable(float x, float y, float width, float height, const se::Color &color, const ShaderProgram& program, bool isFixed);
  
  virtual ~Renderable();
  
  void Draw(const glm::mat4 &Projection, const glm::mat4 &View) const;
  void GenVertexBuffers();
  float GetX() const;
  float GetY() const;
  float GetWidth() const;
  float GetHeight() const;
  se::Color GetColor() const;
  float GetAngle();
  glm::mat4 GetModelMatrix() const;
  void SetX(float x);
  void SetY(float y);
  void SetColor(se::Color color);
  void SetWidth(float width);
  void SetHeight(float height);
  void SetFixedMode(bool isFixed);
  void SetAngle(float angle);
  void SetShaderProgram(const ShaderProgram &program);
  void Move(float offsetX, float offsetY);
  void Rotate(float newAngle);
  void FlipX(bool isFlip);
  void FlipY(bool isFlip);
  bool IsFlippedX() const;
  bool IsFlippedY() const;

protected:
  bool isFlippedX;
  bool isFlippedY;
  float x;
  float y;
  float width;
  float height;
  float angle;

  GLfloat vertex_buffer[18];
  GLuint vertexbuffer;
  
  se::Color color;
  ShaderProgram program;
  mutable glm::mat4 matrix;
  mutable bool positionChanged;
  bool isFixed;
  bool flipped;
};
