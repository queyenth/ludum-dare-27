#include "Shader.h"

#include <stdio.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

Shader::Shader() {}

Shader::Shader(std::string filepath, GLenum type) : filepath(filepath), type(type) {
}

Shader::~Shader() {
  glDeleteShader(shader);
}

void Shader::SetFile(std::string filepath) {
  this->filepath = filepath;
}

void Shader::SetShaderID(GLenum type) {
  this->type = type;
}

void Shader::Compile() {
  shader = glCreateShader(type);
  std::string shaderCode;
  std::ifstream shaderStream(filepath, std::ios::in);
  if (shaderStream.is_open()) {
    std::string Line = "";
    while (std::getline(shaderStream, Line))
      shaderCode += "\n" + Line;
    shaderStream.close();
  }
  else {
    fprintf(stderr, "IMPOSIBLU to open %s. Are you in the right directory?", filepath.c_str());
    shader = 0;
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  fprintf(stdout, "Compiling shader: %s\n", filepath);
  char const * sourcePointer = shaderCode.c_str();
  glShaderSource(shader, 1, &sourcePointer, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> shaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(shader, InfoLogLength, NULL, &shaderErrorMessage[0]);
    fprintf(stderr, "%s\n", &shaderErrorMessage[0]);
  }
}