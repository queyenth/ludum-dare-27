#include "Screen.h"

Screen::~Screen() {
  KillWindow();
}

Screen::Screen() : fullscreen(false), nameOfWindow("PlatOK software") {
  isActive = true;
  isOpened = true;
}

Screen::Screen(int width, int height, string name, bool fullscreen) {
  this->width = width;
  this->height = height;
  this->fullscreen = fullscreen;
  isActive = true;
  nameOfWindow = name;
  isOpened = true;
  CreateUserWindow();
}

void Screen::CreateUserWindow() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(-1);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, nameOfWindow.c_str(), NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible\n");
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    exit(-1);
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  input.SetWindow(window);

  ResizeWindow(width, height);
  InitGL();
}

void Screen::ProcessEvents() {
  glfwPollEvents();
}

void Screen::ResizeWindow(int width, int height) {
  glViewport(0, 0, width, height);
}

void Screen::KillWindow() {
  glfwTerminate();
}

bool Screen::IsOpened() const {
  return !glfwWindowShouldClose(window);
}

bool Screen::IsActive() const {
  return isActive;
}

void Screen::InitGL() {
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  glShadeModel(GL_SMOOTH);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glViewport(0, 0, width, height);
  glOrtho(0, width, 0, height, 0, 1);
}

void Screen::SwitchBuffers() {
  glfwSwapBuffers(window);
}

void Screen::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Input& Screen::GetInput() {
  return input;
}

int Screen::GetWidth() const {
	return width;
}

int Screen::GetHeight() const {
	return height;
}

void Screen::SetWidth(int width) {
  this->width = width;
}

void Screen::SetHeight(int height) {
  this->height = height;
}

void Screen::SetFullscreen(bool fullscreen) {
  this->fullscreen = fullscreen;
}

string Screen::GetName() const {
	return nameOfWindow;
}