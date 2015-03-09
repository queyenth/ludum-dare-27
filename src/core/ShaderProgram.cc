#include <stdio.h>

#include <vector>

#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram() {}

ShaderProgram::ShaderProgram(Shader& vertexShader, Shader& fragmentShader) {
  Link(vertexShader, fragmentShader);
}

ShaderProgram::~ShaderProgram() {}

void ShaderProgram::Link(Shader& vertexShader, Shader& fragmentShader) {
  vertexShader.Compile();
  fragmentShader.Compile();
  fprintf(stdout, "Linking program\n");
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint Result;
  int InfoLogLength;

  glGetProgramiv(program, GL_LINK_STATUS, &Result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> errorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(program, InfoLogLength, NULL, &errorMessage[0]);
    fprintf(stderr, "%s\n", &errorMessage[0]);
  }
}