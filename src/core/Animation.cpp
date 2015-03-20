#include "Animation.h"

#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

Animation::Animation() : Renderable(), currentFrame(0), speed(0.1), startAnimation(0) {
}


Animation::Animation(float x, float y, float width, float height, const ShaderProgram& program, bool isFixed) :
  Renderable(x, y, width, height, program, isFixed), currentFrame(0), speed(0.1), startAnimation(0) {
}

Animation::~Animation() {
  for (auto i : uvbuffer)
    if (glIsBuffer(i))
      glDeleteBuffers(1, &i);
}
  
void Animation::Draw(const Texture &texture, const glm::mat4 &Projection, const glm::mat4 &View) {
  if (!texture.IsValid() || !width || !height || !textureRects.size()) return;

  if (positionChanged) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(x+width/2, y+height/2, 0.0f));
    matrix = glm::rotate(T, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));
    positionChanged = false;
  }

  if (flipped) {
    for (int i = 0; i < textureRects.size(); i++) {
      if (isFlippedY)
        std::swap(textureRects[i].bottom, textureRects[i].top);
      if (isFlippedX)
        std::swap(textureRects[i].left, textureRects[i].right);
    }
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
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[currentFrame]);
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

  if (currentFrame+1 < textureRects.size()) {
    if (glfwGetTime() - startAnimation >= speed) {
      currentFrame++;
      startAnimation = glfwGetTime();
    }
  }
  else if (loop && glfwGetTime() - startAnimation >= speed) {
    currentFrame = 0;
    startAnimation = glfwGetTime();
  }
}

int Animation::GetCurrentFrame() const {
  return currentFrame;
}

void Animation::SetLoopMode(bool loop) {
  this->loop = loop;
}

void Animation::SetCurrentFrame(int currentFrame) {
  this->currentFrame = currentFrame;
}

void Animation::SetSpeed(double speed) {
  this->speed = speed;
}

void Animation::AddFrame(const Texture &texture, Rect textureRect) {
  textureRects.push_back(Rect(textureRect.left/texture.GetWidth(), textureRect.top/texture.GetHeight(), textureRect.right/texture.GetWidth(), textureRect.bottom/texture.GetHeight()));
}

void Animation::GenUVBuffers() {
  for (int i = 0; i < uv_buffers.size(); i++) {
    uv_buffers[i].clear();
  }
  uv_buffers.clear();
  for (int i = 0; i < uvbuffer.size(); i++) {
    if (glIsBuffer(uvbuffer[i])) {
      glDeleteBuffers(1, &uvbuffer[i]);
    }
  }
  uvbuffer.clear();
  for (int i = 0; i < textureRects.size(); i++) {
    std::vector<GLfloat> uv_buffer(12);
    uv_buffer[0] = uv_buffer[8] = uv_buffer[10] = textureRects[i].left;
    uv_buffer[1] = uv_buffer[3] = uv_buffer[11] = textureRects[i].top;
    uv_buffer[2] = uv_buffer[4] = uv_buffer[6] = textureRects[i].right;
    uv_buffer[5] = uv_buffer[7] = uv_buffer[9] = textureRects[i].bottom;
    uv_buffers.push_back(uv_buffer);

    uvbuffer.push_back(0);
    if (glIsBuffer(uvbuffer[i]))
      glDeleteBuffers(1, &uvbuffer[i]);
    glGenBuffers(1, &uvbuffer[i]);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[i]);
    glBufferData(GL_ARRAY_BUFFER, uv_buffers[i].size() * sizeof(GLfloat), &uv_buffers[i][0], GL_STATIC_DRAW);
  }

}
void Animation::AddFrame(const Texture &texture) {
  Rect textureRect = Rect(texture);
  textureRects.push_back(Rect(textureRect.left/texture.GetWidth(), textureRect.top/texture.GetHeight(), textureRect.right/texture.GetWidth(), textureRect.bottom/texture.GetHeight()));
}