#pragma once
#include <string>
#include <Windows.h>
#include <gl/GL.h>
#include <SOIL.h>

class Texture {
public:
  Texture();
  ~Texture();
  
  bool LoadFromFile(std::string imagePath);
  void BindTexture() const;
  int GetWidth() const;
  int GetHeight() const;
  bool IsValid() const;
private:
  GLuint image;
  int width;
  int height;
  bool isValid;
};