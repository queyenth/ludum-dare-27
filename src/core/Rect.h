#pragma once

#include "Texture.h"

class Rect {
public:
  
  Rect() {
    left = top = right = bottom = 0.0f;
  }
  
  Rect(float left, float top, float right, float bottom) {
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
  }

  Rect(const Texture &texture) {
    this->left = 0;
    this->right = texture.GetWidth();
    this->top = 0;
    this->bottom = texture.GetHeight();
  }

  float left;
  float top;
  float right;
  float bottom;
};