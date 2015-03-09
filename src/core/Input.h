#pragma once

#include <GLFW/glfw3.h>

class Input {
public:
  
  Input();
  Input(GLFWwindow *window);
  ~Input();

  bool IsKeyPressed(int) const;
  void GetMouse(double*, double*) const;
  bool IsMousePressed(int) const;
  void SetWindow(GLFWwindow*);
private:
  GLFWwindow* window;
};