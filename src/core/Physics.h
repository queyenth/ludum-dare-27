#pragma once

#include "Rect.h"

class Physics {
public:
  Physics() {}

  static bool CheckCollide(const Rect first, const Rect second) {
    return (first.left < second.right && first.top < second.bottom && second.left < first.right && second.top < first.bottom);
  }
};