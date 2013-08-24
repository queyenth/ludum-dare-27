#pragma once

#include "Input.h"
#include "Color.h"

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace std;

class Screen {
public:

  Screen();
  Screen(wstring name, int width=640, int height=480, bool fullscreen=false, int bits=32);

  ~Screen();

  void CreateUserWindow();
  void SwitchBuffers();
  void Clear(se::Color color = se::Color(1.0f, 1.0f, 1.0f));
  void ProcessEvents();
  bool IsOpened() const;
  bool IsActive() const;
  Input& GetInput();
  int GetWidth() const;
  int GetHeight() const;
  void SetWidth(int width);
  void SetHeight(int height);
  void SetFullscreen(bool fullscreen);
  wstring GetName() const;
  HWND GetHandle() const;
  HDC GetDC() const;
  void KillWindow();
  void ResizeWindow(int width, int height);
private:
  
  void InitGL();
  void ProcessEvents(UINT uMsg, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  Input input;
  HINSTANCE hInstance;
  int width;
  int height;
  int bits;
  bool isOpened;
  bool fullscreen;
  bool isActive;
  wstring nameOfWindow;
  HWND hWnd;
  HDC hDC;
  HGLRC hRC;
};