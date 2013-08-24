#pragma once
#include <vector>

#include "Texture.h"
#include "Color.h"
#include "Rect.h"
#include "Matrix.h"

class Animation {
public:
  Animation();
  Animation(float x, float y, int width, int height, se::Color color, bool isFixed);
  
  virtual ~Animation();
  
  void Draw(const Texture &texture) const;
  float GetX() const;
  float GetY() const;
  int GetWidth() const;
  int GetHeight() const;
  se::Color GetColor() const;
  double GetAngle();
  int GetCurrentFrame() const;
  void SetX(float x);
  void SetY(float y);
  void SetColor(se::Color color);
  void SetWidth(int width);
  void SetHeight(int height);
  void SetFixedMode(bool isFixed);
  void SetLoopMode(bool loop);
  void SetAngle(double angle);
  void SetSpeed(DWORD speed);
  void SetCurrentFrame(int currentFrame);
  void AddFrame(Rect textureRect, const Texture &texture);
  void Move(float offsetX, float offsetY);
  void Rotate(double newAngle);
  void FlipX(bool isFlip);
  void FlipY(bool isFlip);
  bool IsFlippedX() const;
  bool IsFlippedY() const;

private:
  std::vector<Rect> textureRects;
  DWORD speed;
  mutable int currentFrame;
  bool loop;
  
  bool isFlippedX;
  bool isFlippedY;
  float x;
  float y;
  int width;
  int height;
  se::Color color;
  mutable Matrix matrix;
  mutable bool positionChanged;
  double angle;
  bool isFixed;
};
