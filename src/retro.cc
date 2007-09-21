#include <remar2d.h>
#include "Input.h"
#include "GameLogic.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int BPP = 32;
const int FS = 0;

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  remar2d *gfx = new remar2d(WIDTH, HEIGHT, BPP, FS);

  Input *input = new Input();

  GameLogic *gameLogic = new GameLogic(input, gfx);

  while(gameLogic->quit() == false)
    {
      input->getInput();

      gameLogic->update();

      gfx->redraw();

      SDL_Delay(5);
    }

  delete gfx;
  delete input;
  delete gameLogic;

  SDL_Quit();
}
