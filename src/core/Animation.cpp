#include "Animation.h"

Animation::Animation() : x(0), y(0), width(0), height(0), angle(0), positionChanged(true),
isFlippedX(false), isFlippedY(false), isFixed(false), color(se::Color()), currentFrame(0), speed(500) {
}


Animation::Animation(float x, float y, int width, int height, se::Color color, bool isFixed) : x(x), y(y), width(width), height(height), angle(0), positionChanged(true),
isFlippedX(false), isFlippedY(false), isFixed(isFixed), color(color), currentFrame(0), speed(500) {
}

Animation::~Animation(){}
  
void Animation::Draw(const Texture &texture) const {
  static DWORD test = 0;
  if (!texture.IsValid() || !width || !height || !textureRects.size()) return;

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

  float top = isFlippedY ? textureRects[currentFrame].bottom : textureRects[currentFrame].top;
  float left = isFlippedX ? textureRects[currentFrame].right : textureRects[currentFrame].left;
  float right = isFlippedX ? textureRects[currentFrame].left : textureRects[currentFrame].right;
  float bottom = isFlippedY ? textureRects[currentFrame].top : textureRects[currentFrame].bottom;

  glEnable(GL_TEXTURE_2D);
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

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  if (currentFrame+1 < textureRects.size()) {
    if (GetTickCount() - test >= speed) {
      currentFrame++;
      test = GetTickCount();
    }
  }
  else if (loop && GetTickCount() - test >= speed) {
    currentFrame = 0;
    test = GetTickCount();
  }
}

float Animation::GetX() const {
  return x;
}

float Animation::GetY() const {
  return y;
}

int Animation::GetWidth() const {
  return width;
}

int Animation::GetHeight() const {
  return height;
}

se::Color Animation::GetColor() const {
  return color;
}

double Animation::GetAngle() {
  return angle;
}

int Animation::GetCurrentFrame() const {
  return currentFrame;
}

void Animation::SetX(float x) {
  this->x = x;
  positionChanged = true;
}

void Animation::SetY(float y) {
  this->y = y;
  positionChanged = true;
}

void Animation::SetColor(se::Color color) {
  this->color = color;
}

void Animation::SetWidth(int width) {
  this->width = width;
}

void Animation::SetHeight(int height) {
  this->height = height;
}

void Animation::SetFixedMode(bool isFixed) {
  this->isFixed = isFixed;
}

void Animation::SetLoopMode(bool loop) {
  this->loop = loop;
}

void Animation::SetAngle(double angle) {
  this->angle = angle;
}

void Animation::SetCurrentFrame(int currentFrame) {
  this->currentFrame = currentFrame;
}

void Animation::SetSpeed(DWORD speed) {
  this->speed = speed;
}

void Animation::AddFrame(Rect textureRect, const Texture &texture) {
  textureRects.push_back(Rect(textureRect.left/texture.GetWidth(), textureRect.top/texture.GetHeight(), textureRect.right/texture.GetWidth(), textureRect.bottom/texture.GetHeight()));
}

void Animation::Move(float offsetX, float offsetY) {
  SetX(x+offsetX);
  SetY(y+offsetY);
}

void Animation::Rotate(double newAngle) {
  angle += newAngle;
  positionChanged = true;
}

void Animation::FlipX(bool isFlip) {
  isFlippedX = isFlip;
}

void Animation::FlipY(bool isFlip) {
  isFlippedY = isFlip;
}

bool Animation::IsFlippedX() const {
  return isFlippedX;
}

bool Animation::IsFlippedY() const {
  return isFlippedY;
}