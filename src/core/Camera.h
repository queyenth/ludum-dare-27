#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
public:

  Camera() {
    view = glm::lookAt(
      glm::vec3(0, 0, 2),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0)
    );
  }

  Camera(float x, float y) {
    view = glm::lookAt(
      glm::vec3(0, 0, 2),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0)
    );
    OffsetX(x);
    OffsetY(y);
  }

  inline operator glm::mat4() {
    return view;
  }

  glm::mat4 GetMatrix() {
    return view;
  }

  void OffsetX(float x) {
    view = glm::translate(view, glm::vec3(x, 0, 0));
  }

  void OffsetY(float y) {
    view = glm::translate(view, glm::vec3(0, y, 0));
  }

private:
  glm::mat4 view;
};