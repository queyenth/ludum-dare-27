#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Input.h"
#include "Color.h"

using namespace std;

class Screen {
public:

  Screen();
  Screen(int width, int height, string name, bool fullscreen=false);

  ~Screen();

  void CreateUserWindow();
  void SwitchBuffers();
  void Clear();
  void ProcessEvents();
  bool IsOpened() const;
  bool IsActive() const;
  Input& GetInput();
  int GetWidth() const;
  int GetHeight() const;
  void SetWidth(int width);
  void SetHeight(int height);
  void SetFullscreen(bool fullscreen);
  string GetName() const;
  void KillWindow();
  void ResizeWindow(int width, int height);
private:
  
  void InitGL();

  GLFWwindow *window;

  Input input;
  int width;
  int height;
  bool isOpened;
  bool fullscreen;
  bool isActive;
  string nameOfWindow;
};