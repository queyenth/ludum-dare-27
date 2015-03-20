#include "Renderable.h"

#include <gtc/matrix_transform.hpp>

Renderable::Renderable() : x(0), y(0), width(0), height(0), angle(0), positionChanged(true),
isFlippedX(false), isFlippedY(false), isFixed(false) {
}


Renderable::Renderable(float x, float y, float width, float height, const ShaderProgram& program,
  bool isFixed) : x(x), y(y), width(width), height(height), angle(0), positionChanged(true),
  isFlippedX(false), isFlippedY(false), isFixed(isFixed), program(&program) {
}

Renderable::~Renderable() {
  if (glIsBuffer(vertexbuffer))
    glDeleteBuffers(1, &vertexbuffer);
  if (glIsBuffer(normalbuffer))
    glDeleteBuffers(1, &normalbuffer);
}

void Renderable::Draw(const glm::mat4 &Projection, const glm::mat4 &View) const {
  if (!width || !height || !glIsBuffer(vertexbuffer)) return;

  if (positionChanged) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(x+width/2, y+height/2, 0.0f));
    matrix = glm::rotate(T, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));
    positionChanged = false;
  }

  GLuint MatrixID = glGetUniformLocation(*program, "MVP");

  glm::mat4 MVP = Projection * View * matrix;

  glUseProgram(*program);

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

float Renderable::GetAngle() {
  return angle;
}

glm::mat4 Renderable::GetModelMatrix() const {
  return matrix;
}

ShaderProgram Renderable::GetShaderProgram() const {
  return *program;
}

Rect Renderable::GetRect() const {
  return Rect(x, y, x + width, y + height);
}

void Renderable::SetX(float x) {
  this->x = x;
  positionChanged = true;
}

void Renderable::SetY(float y) {
  this->y = y;
  positionChanged = true;
}

void Renderable::SetWidth(float width) {
  this->width = width;
}

void Renderable::SetHeight(float height) {
  this->height = height;
}

void Renderable::GenVertexBuffers() {
  /*
  * Should generate normals too
  * Our renderable object is two triangles,
  * v1(-(width/2), -(height/2))
  * v2(width/2, -(height/2))
  * v3(-(width/2), height/2)
  * v4(width/2, height/2)
  * t1(v1, v2, v4) and t2(v4, v3, v1)
  * T1edge1 = T1v1 - T1v2
  * T1edge2 = T1v3 - T1v1
  * T1normal = cross(T1edge1, T1edge2).normalize()
  * T2edge1 = T2v1 - T2v2
  * T2edge2 = T2v3 - T2v1
  * T2normal = cross(T2edge1, T2edge2).normalize()
  * v1.normal = normalize(T1normal + T2normal)
  * v4.normal = normalize(T1normal + T2normal)
  */

  glm::vec3 v1(-(width / 2), -(height / 2), 0.0f);
  glm::vec3 v2(width / 2, -(height / 2), 0.0f);
  glm::vec3 v3(-(width / 2), height / 2, 0.0f);
  glm::vec3 v4(width / 2, height / 2, 0.0f);

  glm::vec3 T1edge1 = v1 - v2;
  glm::vec3 T1edge2 = v4 - v1;
  glm::vec3 T2edge1 = v4 - v3;
  glm::vec3 T2edge2 = v1 - v4;

  glm::vec3 T1normal = glm::normalize(glm::cross(T1edge1, T1edge2));
  glm::vec3 T2normal = glm::normalize(glm::cross(T2edge1, T2edge2));

  glm::vec3 v1normal = glm::normalize(T1normal + T2normal);
  glm::vec3 v2normal = glm::normalize(T1normal + T2normal);
  glm::vec3 v3normal = glm::normalize(T1normal + T2normal);
  glm::vec3 v4normal = glm::normalize(T1normal + T2normal);

  normal_buffer[0] = normal_buffer[12] = normal_buffer[15] = T1normal.x;
  normal_buffer[1] = normal_buffer[4] = normal_buffer[16] = T1normal.y;
  normal_buffer[3] = normal_buffer[6] = normal_buffer[9] = T2normal.x;
  normal_buffer[7] = normal_buffer[10] = normal_buffer[13] = T2normal.y;
  normal_buffer[2] = normal_buffer[5]
    = normal_buffer[8] = normal_buffer[11]
    = normal_buffer[14] = normal_buffer[17] = T1normal.z;

  if (glIsBuffer(normalbuffer))
    glDeleteBuffers(1, &normalbuffer);
  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer), normal_buffer, GL_STATIC_DRAW);

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
  this->program = &program;
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

void Renderable::CopyLocation(Renderable *other) {
  this->x = other->x;
  this->y = other->y;
  positionChanged = true;
}