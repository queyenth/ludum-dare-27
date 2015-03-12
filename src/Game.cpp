#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "core/Screen.h"
#include "core/Sprite.h"
#include "core/Animation.h"

const int TENSECONDS = 10*1000;
const float WIDTH = 1024;
const float HEIGHT = 768;

void DrawLogo(Screen &screen) {
  Texture logoTexture;
  ShaderProgram program = ShaderProgram(
    Shader("LogoShader.vert", GL_VERTEX_SHADER),
    Shader("LogoShader.frag", GL_FRAGMENT_SHADER)
  );
  double startTime = glfwGetTime();

  logoTexture.LoadFromFile("img/logo.png");
  if (!logoTexture.IsValid()) {
    exit(-1);
  }

  glm::mat4 Projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, 0.1f, 100.f);
  glm::mat4 View = glm::lookAt(
    glm::vec3(0, 0, 2),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0)
  );

  float x = (screen.GetWidth() - logoTexture.GetWidth()) / 2;
  float y = (screen.GetHeight() - logoTexture.GetHeight()) / 2;
  Sprite logo(x, y, logoTexture.GetWidth(), logoTexture.GetHeight(), se::Color(), program, true);
  logo.GenVertexBuffers();
  logo.SetTexture(logoTexture);
  logo.GenUVBuffers();
  screen.SetClearColor(se::Color(1.0f, 1.0f, 1.0f));
  
  GLuint alphaID = glGetUniformLocation(program, "alphaChannel");

  GLfloat alpha = 0.0f;

  double currentTime = startTime;

  while (glfwGetTime() - startTime < 3 && screen.IsOpened()) {
    screen.Clear();
    
    if (glfwGetTime() - currentTime >= 0.05) {
      alpha += 0.024f;
      currentTime = glfwGetTime();
    }

    glUniform1f(alphaID, alpha);
    logo.Draw(logoTexture, Projection, View);

    screen.SwitchBuffers();
    screen.ProcessEvents();
    Sleep(10);
  }
}

void DrawAnimation(Screen &screen) {
  Texture texture;
  Input input = screen.GetInput();
  ShaderProgram program = ShaderProgram(
    Shader("VertexShader.vert", GL_VERTEX_SHADER),
    Shader("FragmentShader.frag", GL_FRAGMENT_SHADER)
  );
  double startTime = glfwGetTime();

  texture.LoadFromFile("img/spritesheet.png");
  if (!texture.IsValid()) {
    exit(-1);
  }

  glm::mat4 Projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, 0.1f, 100.f);
  glm::mat4 View = glm::lookAt(
    glm::vec3(0, 0, 2),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0)
  );

  Animation animation(0, 0, 32, 64, se::Color(), program, false);
  animation.AddFrame(texture, Rect(0, 133, 31, 197));
  animation.AddFrame(texture, Rect(33, 133, 65, 197));
  animation.AddFrame(texture, Rect(67, 133, 98, 197));
  animation.AddFrame(texture, Rect(100, 133, 131, 197));
  animation.SetLoopMode(true);
  animation.GenVertexBuffers();
  animation.GenUVBuffers();

  screen.SetClearColor(se::Color(1.0f, 1.0f, 1.0f));
  
  while (!input.IsKeyPressed(GLFW_KEY_ESCAPE) && screen.IsOpened()) {
    screen.Clear();
    
    animation.Draw(texture, Projection, View);

    if (input.IsKeyPressed(GLFW_KEY_LEFT)) {
      animation.Move(-5, 0);
    }
    else if (input.IsKeyPressed(GLFW_KEY_RIGHT)) {
      animation.Move(5, 0);
    }
    if (input.IsKeyPressed(GLFW_KEY_UP)) {
      animation.Move(0, -5);
    }
    else if (input.IsKeyPressed(GLFW_KEY_DOWN)) {
      animation.Move(0, 5);
    }

    if (input.IsKeyPressed(GLFW_KEY_F1)) {
      animation.FlipX(true);
    }

    screen.SwitchBuffers();
    screen.ProcessEvents();
    Sleep(10);
  }
}

int main() {
  Screen screen(WIDTH, HEIGHT, "PlatOK Software");
  Input &input = screen.GetInput();

  Texture texture;
  if (!texture.LoadFromFile("img/spritesheet.png"))
    exit(1);

  ShaderProgram program = ShaderProgram(
    Shader("VertexShader.vert", GL_VERTEX_SHADER),
    Shader("FragmentShader.frag", GL_FRAGMENT_SHADER)
  );

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  glm::mat4 Projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, 0.1f, 100.f);
  glm::mat4 View = glm::lookAt(
    glm::vec3(0, 0, 2),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0)
  );

  Texture backgroundTexture;
  backgroundTexture.LoadFromFile("img/back.png");
  if (!backgroundTexture) {
    exit(-1);
  }
  Sprite background(0, 0, screen.GetWidth(), screen.GetHeight(), se::Color(), program, true);
  background.GenVertexBuffers();
  background.SetTexture(backgroundTexture);
  background.GenUVBuffers();

  //DrawLogo(screen);
  DrawAnimation(screen);

  while (screen.IsOpened()) {
    screen.Clear();
    //background.Draw(backgroundTexture, Projection, View);
    screen.SwitchBuffers();
    screen.ProcessEvents();
    Sleep(15);
  }
  return 0;
}