#pragma once
#include "Matrix.h"
#include "Rect.h"
#include "Texture.h"
#include "Color.h"

#include <Windows.h>
#include <gl/GL.h>

class Text {
public:
  Text() {}
  
  ~Text() {}
  
  static void Draw(float x, float y, se::Color color, std::string printString, const Texture &texture) {
    if (!texture.IsValid() || !printString.length()) return;
    
    if (color.a != 0.0f)
      glColor4f(color.r, color.g, color.b, color.a);
    else
      glColor3f(color.r, color.g, color.b);

    // A = {0, 32, 32, 0}
    // B = {34, 32, 66, 0}
    for (auto i = 0; i < printString.size(); i++) {
      Matrix matrix;
      matrix.Set(12, x+i*32);
      matrix.Set(13, y);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glMultMatrixf(matrix.GetMatrix());
      Rect textureRect((32*(printString[i]-65)+2*i)/texture.GetWidth(), 32/texture.GetHeight(), ((32*(printString[i]-65)+2*i)+32)/texture.GetWidth(), 0);
      
      float top = textureRect.top;
      float left = textureRect.left;
      float right = textureRect.right;
      float bottom = textureRect.bottom;

      glEnable(GL_TEXTURE_2D);
      texture.BindTexture();
      glBegin(GL_QUADS);
        glTexCoord2f(left, top);
        glVertex2f(0, 0);
      
        glTexCoord2f(right, top);
        glVertex2f(32, 0);

        glTexCoord2f(right, bottom);
        glVertex2f(32, 32);
      
        glTexCoord2f(left, bottom);
        glVertex2f(0, 32);
      glEnd();
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
};