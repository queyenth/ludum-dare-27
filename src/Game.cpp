#include "core/Screen.h"
#include "core/Texture.h"
#include "core/Sprite.h"
#include "core/Animation.h"
#include "core/Level.h"
#include "core/Player.h"
#include "core/Physics.h"
#include "core/Text.h"

const int TENSECONDS = 10*1000;

int main() {
  Screen screen(L"Ludum dare 27", 32*16, 32*16);
  Input &input = screen.GetInput();
  Texture texture;
  if (!texture.LoadFromFile("img\\spritesheet.png"))
    exit(1);
  Level level(texture);
  Player player(texture);
  Sprite background(0, 0, 32*16, 32*16, se::Color(), true);
  background.SetTextureRect(Rect(0, 433, 512, 945), texture);
  Animation door(7*32, 0, 32, 64, se::Color(), true);
  door.AddFrame(Rect(132, 265, 164, 200), texture);
  door.AddFrame(Rect(99, 265, 131, 200), texture);
  door.AddFrame(Rect(66, 265, 98, 200), texture);
  door.AddFrame(Rect(33, 265, 65, 200), texture);
  door.AddFrame(Rect(0, 265, 32, 200), texture);
  door.SetLoopMode(false);

  level.GenerateLevel();
  int seconds = GetTickCount();
  char text[100];
  while (screen.IsOpened()) {
    screen.ProcessEvents();
    
    if (input.IsKeyPressed(VK_F1)) level.GenerateLevel();
    
    // ≈сли нажата клавиша влево, двигайс€ влево
    if (input.IsKeyPressed(VK_LEFT)) {
      player.sprite.Move(-8.0f, 0.0f);
      if (player.CheckCollide(level, Level::BLOCK)) {
        player.sprite.Move(8.0f, 0.0f);
      }
    }
    // ј если нажата вправо - двигайс€ вправо
    else if (input.IsKeyPressed(VK_RIGHT)) {
      player.sprite.Move(8.0f, 0.0f);
      if (player.CheckCollide(level, Level::BLOCK)) {
        player.sprite.Move(-8.0f, 0.0f);
      }
    }
    else if (player.currentState != Player::JUMP && player.currentState != Player::FALL)
      player.currentState = Player::STADE;
    
    if (input.IsKeyPressed(VK_UP)) player.Jump(texture);
    
    Rect playerRect = Rect(player.sprite.GetX(), player.sprite.GetY(), player.sprite.GetX()+player.sprite.GetWidth(), player.sprite.GetY()+player.sprite.GetHeight());
    Rect exitRect = Rect(7*32, 0, 8*32, 64);
    
    player.Update(level);

    screen.Clear();
    
    background.Draw(texture);
    level.Draw(texture);
    player.Draw(texture);
    sprintf(text, "TIME %d", (TENSECONDS - (GetTickCount()-seconds))/1000);
    Text::Draw(32*4, 32*15, se::Color(1.0f, 0.0f, 0.0f, 1.0f), text, texture);
    sprintf(text, "POINTS %d", player.experience);
    Text::Draw(32*4, 32*14, se::Color(), text, texture);
    if (level.guards == 0) {
      door.Draw(texture);
    }
    if ((level.guards == 0 && Physics::CheckCollide(playerRect, exitRect)) || GetTickCount() - seconds >= TENSECONDS-1000) {
      level.GenerateLevel();
      int prevExp = player.experience;
      player = Player(texture);
      player.experience = (GetTickCount() - seconds)/1000 + prevExp + 1;
      seconds = GetTickCount();
      door.SetCurrentFrame(0);
    }
    screen.SwitchBuffers();
    Sleep(15);
  }
  return 0;
}