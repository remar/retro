#include <remar2d.h>
#include "Input.h"
#include "GameLogic.h"
#include "SDL_mixer.h"
#include "SoundManager.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int BPP = 32;
const int FS = 0;

int main()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024))
    {
      printf("SDL_mixer says: %s\n", Mix_GetError());
    }

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
