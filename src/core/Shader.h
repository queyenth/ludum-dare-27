#pragma once

#include <string>

#include <GL/glew.h>

class Shader {
public:
  Shader();
  Shader(std::string filepath, GLenum type);

  ~Shader();

  void SetFile(std::string filepath);
  void Compile();
  void SetShaderID(GLenum type);

  inline operator GLuint() const {
    return shader;
  }
private:
  GLuint shader;
  std::string filepath;
  GLenum type;
};