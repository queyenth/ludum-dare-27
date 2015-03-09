#pragma once

#include <GL/glew.h>

class Shader;

class ShaderProgram {
public:
  ShaderProgram();
  ShaderProgram(Shader &vertexShader, Shader &fragmentShader);

  ~ShaderProgram();

  void Link(Shader &vertexShader, Shader &fragmentShader);

  inline operator GLuint() const {
    return program;
  }
private:
  GLuint program;
};