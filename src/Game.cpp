#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "core/Screen.h"
#include "core/Sprite.h"
#include "core/Animation.h"
#include "core/Physics.h"
#include "core/Player.h"
#include "core/Level.h"
#include "core/Camera.h"

void DrawLogo(Screen &screen) {
  Texture logoTexture;
  ShaderProgram program = ShaderProgram(
    Shader("LogoShader.vert", GL_VERTEX_SHADER),
    Shader("LogoShader.frag", GL_FRAGMENT_SHADER)
  );
  Input input = screen.GetInput();
  double startTime = glfwGetTime();

  logoTexture.LoadFromFile("img/logo.png");
  if (!logoTexture.IsValid()) {
    exit(-1);
  }

  glm::mat4 Projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, 0.1f, 100.f);
  Camera View = Camera();

  float x = (screen.GetWidth() - logoTexture.GetWidth()) / 2;
  float y = (screen.GetHeight() - logoTexture.GetHeight()) / 2;
  Sprite logo(x, y, logoTexture.GetWidth(), logoTexture.GetHeight(), program, true);
  logo.GenVertexBuffers();
  logo.SetTexture(logoTexture);
  logo.GenUVBuffers();
  screen.SetClearColor(se::Color(1.0f, 1.0f, 1.0f));
  
  GLuint alphaID = glGetUniformLocation(program, "alphaChannel");

  GLfloat alpha = 0.0f;

  double currentTime = startTime;

  while (glfwGetTime() - startTime < 2 && screen.IsOpened() && !input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
    screen.Clear();
    
    if (glfwGetTime() - currentTime >= 0.05) {
      alpha += 0.025f;
      currentTime = glfwGetTime();
    }

    glUniform1f(alphaID, alpha);
    logo.Draw(logoTexture, Projection, View);

    screen.SwitchBuffers();
    screen.ProcessEvents();
  }
}

void DrawAnimation(Screen &screen) {
  Texture texture;
  Input input = screen.GetInput();
  ShaderProgram program = ShaderProgram(
    Shader("AnimationShader.vert", GL_VERTEX_SHADER),
    Shader("AnimationShader.frag", GL_FRAGMENT_SHADER)
  );
  double startTime = glfwGetTime();

  texture.LoadFromFile("img/spritesheet.png");
  if (!texture.IsValid()) {
    exit(-1);
  }

  glm::mat4 Projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, 0.1f, 100.f);
  Camera View = Camera();
  Texture backText("img/back.png");
  if (!backText.IsValid()) {
    exit(-1);
  }
  Sprite background(0, 0, WIDTH, HEIGHT, program, true);
  background.SetTexture(backText);
  background.GenVertexBuffers();
  background.GenUVBuffers();

  Level level;
  level.GenLevel(texture, program);

  Player player(0, 0, texture, program);

  screen.SetClearColor(se::Color(1.0f, 1.0f, 1.0f));

  glUseProgram(program);
  GLuint LightID = glGetUniformLocation(program, "LightPosition_worldspace");
  GLuint ModelMatrixID = glGetUniformLocation(program, "M");
  GLuint ViewMatrixID = glGetUniformLocation(program, "V");

  double lastTime = glfwGetTime();
  int frames = 0;

  glm::vec3 lightPos = glm::vec3(WIDTH/2, HEIGHT-64.f, -88);
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View.GetMatrix()[0][0]);
  
  bool leftDir = false;
  bool rightDir = false;

  while (!input.IsKeyPressed(GLFW_KEY_ESCAPE) && screen.IsOpened()) {

    double currentTime = glfwGetTime();
    frames++;
    if (currentTime - lastTime >= 1.0) {
      printf("%f ms/frame\t%d fps\n", 1000.0 / double(frames), frames);
      frames = 0;
      lastTime += 1.0;
    }

    player.Update(level);

    screen.Clear();

    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &background.GetModelMatrix()[0][0]);
    background.Draw(backText, Projection, View);

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &player.GetCurrentAnimation()->GetModelMatrix()[0][0]);
    player.Draw(texture, Projection, View);

    level.Draw(texture, Projection, View);

    if (input.IsKeyPressed(GLFW_KEY_LEFT)) {
      player.GetCurrentAnimation()->Move(-5, 0);
      if (player.GetCurrentAnimation()->GetX() > WIDTH / 2) {
        View.OffsetX(5);
      }
      player.SetState(Player::MOVE);
    }
    else if (input.IsKeyPressed(GLFW_KEY_RIGHT)) {
      player.GetCurrentAnimation()->Move(5, 0);
      if (player.GetCurrentAnimation()->GetX() > WIDTH / 2) {
        View.OffsetX(-5);
      }
      player.SetState(Player::MOVE);
    }
    else {
      if (player.currentState == Player::MOVE) {
        player.currentState = Player::STADE;
      }
    }

    if (input.IsKeyPressed(GLFW_KEY_UP)) {
      player.Jump();
    }

    if (input.IsKeyPressed(GLFW_KEY_F1)) {
      player.GetCurrentAnimation()->FlipX(true);
    }

    screen.SwitchBuffers();
    screen.ProcessEvents();
    Sleep(16);
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

  /*Texture backgroundTexture;
  backgroundTexture.LoadFromFile("img/back.png");
  if (!backgroundTexture) {
    exit(-1);
  }
  Sprite background(0, 0, screen.GetWidth(), screen.GetHeight(), se::Color(), program, true);
  background.GenVertexBuffers();
  background.SetTexture(backgroundTexture);
  background.GenUVBuffers();*/

  DrawLogo(screen);
  DrawAnimation(screen);
  return 0;

  while (screen.IsOpened()) {
    screen.Clear();
    //background.Draw(backgroundTexture, Projection, View);
    screen.SwitchBuffers();
    screen.ProcessEvents();
    Sleep(15);
  }
  return 0;
}