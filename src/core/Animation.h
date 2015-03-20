#pragma once
#include <vector>

#include "Renderable.h"

class Animation : public Renderable {
public:
  Animation();
  Animation(float x, float y, float width, float height, const ShaderProgram& program, bool isFixed);
  
  virtual ~Animation();
  
  void Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View);
  int GetCurrentFrame() const;
  void SetLoopMode(bool loop);
  void SetSpeed(double speed);
  void SetCurrentFrame(int currentFrame);
  void GenUVBuffers();
  void AddFrame(const Texture &texture);
  void AddFrame(const Texture &texture, Rect textureRect);

private:
  std::vector<Rect> textureRects;
  std::vector<std::vector<GLfloat> > uv_buffers;
  std::vector<GLuint> uvbuffer;

  double speed;
  double startAnimation;
  mutable int currentFrame;
  bool loop;
};
