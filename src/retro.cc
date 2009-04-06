#include <remar2d.h>
#include "Input.h"
#include "GameLogic.h"
#include "SDL_mixer.h"
#include "SoundManager.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int BPP = 32;
const int FS = 0;

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  Input *input = new Input();

  remar2d *gfx = new remar2d(WIDTH, HEIGHT, BPP, FS, "Retrobattle");

  SoundManager *sfx = new SoundManager();

  GameLogic *gameLogic = new GameLogic(input, gfx, sfx);

  while(gameLogic->quit() == false)
    {
      input->getInput();

      gameLogic->update();

      gfx->redraw();

      SDL_Delay(5);
    }

  delete gameLogic;
  delete sfx;
  delete gfx;
  delete input;

  Mix_CloseAudio();
  SDL_Quit();
}
