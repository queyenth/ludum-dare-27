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
    
    for (auto i = 0; i < printString.size(); i++) {
      Rect textureRect;

      if (isalpha(printString[i]))
        textureRect = Rect((float)((printString[i]-65)*33)/texture.GetWidth(), 66.0f/texture.GetHeight(), (float)((printString[i]-65)*33+32)/texture.GetWidth(), 34.0f/texture.GetHeight());
      else if (isalnum(printString[i]))
        textureRect = Rect((float)((printString[i]-48)*33)/texture.GetWidth(), 32.0f/texture.GetHeight(), (float)((printString[i]-48)*33+32)/texture.GetWidth(), 0);
      else
        continue;

      glMatrixMode(GL_MODELVIEW);
  
      Matrix matrix;
      matrix.Set(12, x+i*18);
      matrix.Set(13, y);

      glPushMatrix();
      glMultMatrixf(matrix.GetMatrix());

      if (color.a != 0.0f)
        glColor4f(color.r, color.g, color.b, color.a);
      else
        glColor3f(color.r, color.g, color.b);

      float top = textureRect.top;
      float left = textureRect.left;
      float right = textureRect.right;
      float bottom = textureRect.bottom;

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

      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
    }
  }
};