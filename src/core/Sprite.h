#pragma once
#include "Matrix.h"
#include "Rect.h"
#include "Texture.h"
#include "Color.h"

#include <Windows.h>
#include <gl/GL.h>

class Sprite {
public:
  Sprite();
  Sprite(float x, float y, int width, int height, se::Color color, bool isFixed);
  
  virtual ~Sprite();
  
  void Draw(const Texture &texture) const;
  float GetX() const;
  float GetY() const;
  int GetWidth() const;
  int GetHeight() const;
  se::Color GetColor() const;
  double GetAngle();
  void SetX(float x);
  void SetY(float y);
  void SetColor(se::Color color);
  void SetWidth(int width);
  void SetHeight(int height);
  void SetFixedMode(bool isFixed);
  void SetAngle(double angle);
  void SetTextureRect(Rect textureRect, const Texture &texture);
  void Move(float offsetX, float offsetY);
  void Rotate(double newAngle);
  void FlipX(bool isFlip);
  void FlipY(bool isFlip);
  bool IsFlippedX() const;
  bool IsFlippedY() const;
private:
  bool isFlippedX;
  bool isFlippedY;
  Rect textureRect;
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