#pragma once
namespace se {
class Color {
public:
  Color(float r=1.0f, float g=1.0f, float b=1.0f, float a=0.0f);

  bool operator==(Color &right) {
    if (r == right.r && g == right.g && b == right.b && a == right.a)
      return true;
    else
      return false;
  }

  void operator*(Color &right) {
    r *= right.r;
    g *= right.g;
    b *= right.b;
    a *= right.a;
  }

  friend Color &operator*=(Color &left, Color &right) {
    left.r *= right.r;
    left.g *= right.g;
    left.b *= right.b;
    left.a *= right.a;
    return left;
  }
  
  float r;
  float g;
  float b;
  float a;
};
}