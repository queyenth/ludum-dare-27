#include "core/Screen.h"
#include "core/Texture.h"
#include "core/Sprite.h"
#include "core/Animation.h"
#include "core/Level.h"
#include "core/Player.h"
#include "core/Physics.h"

int main() {
  Screen screen(L"Ludum dare 27", 32*16, 32*16);
  Input &input = screen.GetInput();
  Texture texture;
  if (!texture.LoadFromFile("img\\spritesheet.png"))
    exit(1);
  Level level(texture);
  Player player(texture);
  level.GenerateLevel();
  while (screen.IsOpened()) {
    screen.ProcessEvents();
    
    if (input.IsKeyPressed(VK_F1)) level.GenerateLevel();
    
    if (input.IsKeyPressed(VK_LEFT)) player.sprite.Move(-4.0f, 0.0f);
    else if (input.IsKeyPressed(VK_RIGHT)) player.sprite.Move(4.0f, 0.0f);
    
    if (input.IsKeyPressed(VK_UP)) player.Jump();
    
    Rect playerRect = Rect(player.sprite.GetX(), player.sprite.GetY(), player.sprite.GetX()+player.sprite.GetWidth(), player.sprite.GetY()+player.sprite.GetHeight());
    Rect exitRect = Rect(14*32, 0, 15*32, 32);
    if (Physics::CheckCollide(playerRect, exitRect)) {
      level.GenerateLevel();
      player = Player(texture);
    }

    screen.Clear();
    level.Draw(texture);
    player.Draw(texture, level);
    screen.SwitchBuffers();
    Sleep(15);
  }
  return 0;
}