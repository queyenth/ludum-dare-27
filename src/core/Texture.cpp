#include "Texture.h"

Texture::Texture() : image(0), width(0), height(0), isValid(false) {
}

Texture::~Texture() {
}

bool Texture::LoadFromFile(std::string imagePath) {
  isValid = true;
  SOIL_load_image(imagePath.c_str(), &width, &height, NULL, SOIL_LOAD_L);
  image = SOIL_load_OGL_texture
  (
    imagePath.c_str(),
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
  );
  if (!image)
    isValid = false;
  return isValid;
}

void Texture::BindTexture() const {
  glBindTexture(GL_TEXTURE_2D, image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

int Texture::GetWidth() const {
  return width;
}

int Texture::GetHeight() const {
  return height;
}

bool Texture::IsValid() const {
  return isValid;
}