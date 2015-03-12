#include "Renderable.h"

#include <gtc/matrix_transform.hpp>

Renderable::Renderable() : x(0), y(0), width(0), height(0), angle(0), positionChanged(true),
isFlippedX(false), isFlippedY(false), isFixed(false), color(se::Color()) {
}


Renderable::Renderable(float x, float y, float width, float height, const se::Color& color, const ShaderProgram& program,
  bool isFixed) : x(x), y(y), width(width), height(height), angle(0), positionChanged(true),
  isFlippedX(false), isFlippedY(false), isFixed(isFixed), color(color), program(program) {
}

Renderable::~Renderable() {
  if (glIsBuffer(vertexbuffer))
    glDeleteBuffers(1, &vertexbuffer);
}

void Renderable::Draw(const glm::mat4 &Projection, const glm::mat4 &View) const {
  if (!width || !height || !glIsBuffer(vertexbuffer)) return;

  if (positionChanged) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(x+width/2, y+height/2, 0.0f));
    matrix = glm::rotate(T, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));
    positionChanged = false;
  }

  GLuint MatrixID = glGetUniformLocation(program, "MVP");

  glm::mat4 MVP = Projection * View * matrix;

  glUseProgram(program);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void *)0
  );

  glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

  glDisableVertexAttribArray(0);
}

float Renderable::GetX() const {
  return x;
}

float Renderable::GetY() const {
  return y;
}

float Renderable::GetWidth() const {
  return width;
}

float Renderable::GetHeight() const {
  return height;
}

se::Color Renderable::GetColor() const {
  return color;
}

float Renderable::GetAngle() {
  return angle;
}

void Renderable::SetX(float x) {
  this->x = x;
  positionChanged = true;
}

void Renderable::SetY(float y) {
  this->y = y;
  positionChanged = true;
}

void Renderable::SetColor(se::Color color) {
  this->color = color;
}

void Renderable::SetWidth(float width) {
  this->width = width;
}

void Renderable::SetHeight(float height) {
  this->height = height;
}

void Renderable::GenVertexBuffers() {
  vertex_buffer[0] = vertex_buffer[12] = vertex_buffer[15] = -(width/2);
  vertex_buffer[1] = vertex_buffer[4] = vertex_buffer[16] = -(height/2);
  vertex_buffer[3] = vertex_buffer[6] = vertex_buffer[9] = width/2;
  vertex_buffer[7] = vertex_buffer[10] = vertex_buffer[13] = height/2;
  vertex_buffer[2] = vertex_buffer[5]
    = vertex_buffer[8] = vertex_buffer[11]
    = vertex_buffer[14] = vertex_buffer[17] = 0.0f;

  if (glIsBuffer(vertexbuffer))
    glDeleteBuffers(1, &vertexbuffer);
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
}

void Renderable::SetFixedMode(bool isFixed) {
  this->isFixed = isFixed;
}

void Renderable::SetAngle(float angle) {
  this->angle = angle;
  positionChanged = true;
}

void Renderable::SetShaderProgram(const ShaderProgram &program) {
  this->program = program;
}

void Renderable::Move(float offsetX, float offsetY) {
  SetX(x + offsetX);
  SetY(y + offsetY);
}

void Renderable::Rotate(float newAngle) {
  angle += newAngle;
  positionChanged = true;
}

void Renderable::FlipX(bool isFlip) {
  isFlippedX = isFlip;
  flipped = true;
}

void Renderable::FlipY(bool isFlip) {
  isFlippedY = isFlip;
  flipped = true;
}

bool Renderable::IsFlippedX() const {
  return isFlippedX;
}

bool Renderable::IsFlippedY() const {
  return isFlippedY;
}