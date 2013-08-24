#pragma once

class Input {
public:
  
  Input();

  void PressKey(int);
  bool IsKeyPressed(int) const;
  void UnPressKey(int);
  void SetMouseX(int);
  void SetMouseY(int);
  int GetMouseX() const;
  int GetMouseY() const;
  void PressLeftMouse();
  bool IsLeftMousePressed() const;
  void PressRightMouse();
  bool IsRightMousePressed() const;
  void UnPressLeftMouse();
  void UnPressRightMouse();
private:
  
  bool keys[256];

  struct {
    int x,y;
    bool leftButton;
    bool rightButton;
  } mouse;
};