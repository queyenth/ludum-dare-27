#include "Sprite.h"

#include <gtc/matrix_transform.hpp>

Sprite::Sprite() : Renderable() {
}

Sprite::Sprite(float x, float y, float width, float height, const ShaderProgram& program,
  bool isFixed) : Renderable(x, y, width, height, program, isFixed) {
}

Sprite::~Sprite() {
  if (glIsBuffer(uvbuffer))
    glDeleteBuffers(1, &uvbuffer);
}

void Sprite::Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View) {
  if (!texture.IsValid() || !width || !height || !glIsBuffer(vertexbuffer) || !glIsBuffer(uvbuffer)) return;

  glm::mat4 T;
  if (isFixed)
    T = glm::translate(glm::mat4(1.0f), glm::vec3(x + width / 2 - View[3][0], y + height / 2 - View[3][1], 0.0f));
  else
    T = glm::translate(glm::mat4(1.0f), glm::vec3(x + width / 2, y + height / 2, 0.0f));
  matrix = glm::rotate(T, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));

  if (flipped) {
    if (isFlippedY)
      std::swap(textureRect.bottom, textureRect.top);
    if (isFlippedX)
      std::swap(textureRect.left, textureRect.right);
    GenUVBuffers();
    flipped = false;
  }

  GLuint MatrixID = glGetUniformLocation(*program, "MVP");
  GLuint TextureID = glGetUniformLocation(*program, "myTextureSampler");

  glm::mat4 MVP = Projection * View * matrix;

  glUseProgram(*program);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glActiveTexture(GL_TEXTURE0);
  texture.BindTexture();
  glUniform1i(TextureID, 0);

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

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void *)0
  );

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer(
    2,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void *)0
  );

  glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

void Sprite::GenUVBuffers() {
  uv_buffer[0] = uv_buffer[8] = uv_buffer[10] = this->textureRect.left;
  uv_buffer[1] = uv_buffer[3] = uv_buffer[11] = this->textureRect.top;
  uv_buffer[2] = uv_buffer[4] = uv_buffer[6] = this->textureRect.right;
  uv_buffer[5] = uv_buffer[7] = uv_buffer[9] = this->textureRect.bottom;

  if (glIsBuffer(uvbuffer))
    glDeleteBuffers(1, &uvbuffer);
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer), uv_buffer, GL_STATIC_DRAW);
}


void Sprite::SetTexture(const Texture &texture, Rect textureRect) {
  this->textureRect = Rect(textureRect.left/texture.GetWidth(), textureRect.top/texture.GetHeight(), textureRect.right/texture.GetWidth(), textureRect.bottom/texture.GetHeight());
}

void Sprite::SetTexture(const Texture &texture) {
  textureRect = Rect(texture);
  this->textureRect = Rect(textureRect.left/texture.GetWidth(), textureRect.top/texture.GetHeight(), textureRect.right/texture.GetWidth(), textureRect.bottom/texture.GetHeight());
}