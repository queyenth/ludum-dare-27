#include "Sprite.h"

Sprite::Sprite() : x(0), y(0), width(0), height(0), angle(0), positionChanged(true),
isFlippedX(false), isFlippedY(false), isFixed(false), color(se::Color()) {
}


Sprite::Sprite(float x, float y, int width, int height, se::Color color, bool isFixed) : x(x), y(y), width(width), height(height), angle(0), positionChanged(true),
isFlippedX(false), isFlippedY(false), isFixed(isFixed), color(color) {
}

Sprite::~Sprite(){}
  
void Sprite::Draw(const Texture &texture) const {
  if (!texture.IsValid() || !width || !height) return;

  if (positionChanged) {
    float Cos = cosf(angle*(float)(3.14159265/180));
    float Sin = sinf(angle*(float)(3.14159265/180));
    matrix.Set(0, Cos);
    matrix.Set(1, Sin);
    matrix.Set(4, -Sin);
    matrix.Set(5, Cos);
    matrix.Set(12, x);
    matrix.Set(13, y);

    positionChanged = false;
  }
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMultMatrixf(matrix.GetMatrix());
  if (color.a != 0.0f)
    glColor4f(color.r, color.g, color.b, color.a);
  else
    glColor3f(color.r, color.g, color.b);

  float top = isFlippedY ? textureRect.bottom : textureRect.top;
  float left = isFlippedX ? textureRect.right : textureRect.left;
  float right = isFlippedX ? textureRect.left : textureRect.right;
  float bottom = isFlippedY ? textureRect.top : textureRect.bottom;

  if (width && height) {
    texture.BindTexture();
    glBegin(GL_QUADS);
      glTexCoord2f(left, top);
      glVertex2f(0, 0);
      
      glTexCoord2f(right, top);
      glVertex2f(width, 0);

      glTexCoord2f(right, bottom);
      glVertex2f(width, height);
      
      glTexCoord2f(left, bottom);
      glVertex2f(0, height);
    glEnd();
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

float Sprite::GetX() const {
  return x;
}

float Sprite::GetY() const {
  return y;
}

int Sprite::GetWidth() const {
  return width;
}

int Sprite::GetHeight() const {
  return height;
}

se::Color Sprite::GetColor() const {
  return color;
}

double Sprite::GetAngle() {
  return angle;
}

void Sprite::SetX(float x) {
  this->x = x;
  positionChanged = true;
}

void Sprite::SetY(float y) {
  this->y = y;
  positionChanged = true;
}

void Sprite::SetColor(se::Color color) {
  this->color = color;
}

void Sprite::SetWidth(int width) {
  this->width = width;
}

void Sprite::SetHeight(int height) {
  this->height = height;
}

void Sprite::SetFixedMode(bool isFixed) {
  this->isFixed = isFixed;
}

void Sprite::SetAngle(double angle) {
  this->angle = angle;
}

void Sprite::SetTextureRect(Rect textureRect, const Texture &texture) {
  this->textureRect = Rect(textureRect.left/texture.GetWidth(), textureRect.top/texture.GetHeight(), textureRect.right/texture.GetWidth(), textureRect.bottom/texture.GetHeight());
}

void Sprite::Move(float offsetX, float offsetY) {
  SetX(x+offsetX);
  SetY(y+offsetY);
}

void Sprite::Rotate(double newAngle) {
  angle += newAngle;
  positionChanged = true;
}

void Sprite::FlipX(bool isFlip) {
  isFlippedX = isFlip;
}

void Sprite::FlipY(bool isFlip) {
  isFlippedY = isFlip;
}

bool Sprite::IsFlippedX() const {
  return isFlippedX;
}

bool Sprite::IsFlippedY() const {
  return isFlippedY;
}