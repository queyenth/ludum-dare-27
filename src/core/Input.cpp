#include "Input.h"

Input::Input() {
  for (int i = 0; i < 256; i++)
    keys[i] = false;

  mouse.x = 0;
  mouse.y = 0;

  mouse.leftButton = false;
  mouse.rightButton = false;
}

void Input::PressKey(int key) {
  keys[key] = true;
}

void Input::UnPressKey(int key) {
  keys[key] = false;
}

bool Input::IsKeyPressed(int key) const {
  return keys[key];
}

void Input::SetMouseX(int x) {
  mouse.x = x;
}

void Input::SetMouseY(int y) {
  mouse.y = y;
}

int Input::GetMouseX() const {
  return mouse.x;
}

int Input::GetMouseY() const {
  return mouse.y;
}

void Input::PressLeftMouse() {
  mouse.leftButton = true;
}

void Input::PressRightMouse() {
  mouse.rightButton = true;
}

bool Input::IsLeftMousePressed() const {
  return mouse.leftButton;
}

bool Input::IsRightMousePressed() const {
  return mouse.rightButton;
}

void Input::UnPressLeftMouse() {
  mouse.leftButton = false;
}

void Input::UnPressRightMouse() {
  mouse.rightButton = false;
}