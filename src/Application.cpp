#include "../include/Game.h"

int main(int argc, char *argv[]) {

  Game game = Game();
  if(!game.Init()){
    LOG_ERROR("Unable to start game.");
    return 0;
  }
  game.Run();
  game.Clean();  

  return 0;
}
