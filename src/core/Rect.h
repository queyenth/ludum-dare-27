#pragma once

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

  float left;
  float top;
  float right;
  float bottom;
};