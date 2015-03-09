#include "Input.h"

Input::Input() : window(NULL) {
}

Input::Input(GLFWwindow *window) : window(window) {
}

Input::~Input() {}

bool Input::IsKeyPressed(int key) const {
  return glfwGetKey(window, key) == GLFW_PRESS;
}

void Input::GetMouse(double *x, double *y) const {
  glfwGetCursorPos(window, x, y);
}

bool Input::IsMousePressed(int button) const {
  return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

void Input::SetWindow(GLFWwindow* window) {
  this->window = window;
}