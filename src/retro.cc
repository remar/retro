/*
 *    Source from Retrobattle, a NES-like collect-em-up.
 *    Copyright (C) 2010 Andreas Remar
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    Original authors contact info: andreas.remar@gmail.com
 */

#include <remar2d.h>
#include "Input.h"
#include "GameLogic.h"
#include "SoundManager.h"
#include "binreloc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const int BPP = 32;
const int FS = 0;

bool sanity_check_datadir(char *datadir)
{
  char buf[1024];
  struct stat s;

  sprintf(buf, "%s/../levels/1.lev", datadir);

  if(stat(buf, &s) == 0)
    {
      return (s.st_mode & S_IFMT) == S_IFREG;
    }

  return false;
}

char *setup_datadir(int argc, char *argv[])
{
  BrInitError e;
  char *datadir = 0;
  char *alternative = 0;

  if(br_init(&e) == 0)
    {
      printf("Failed to init binreloc! Error code: %d\n", e);
    }

  if(argc > 1)
    {
      alternative = argv[1];
    }

  datadir = br_find_exe_dir(alternative);

  if(!sanity_check_datadir(datadir))
    {
      // Try the default location
      datadir = strdup("/opt/retrobattle");

      if(!sanity_check_datadir(datadir))
	{
	  printf("ERROR: Unable to find game data!\n");
	  printf("       Try providing the path when starting, i.e.:\n");
	  printf("       # retro <PATH_TO_RETROBATTLE>\n");

	  free(datadir);
	  datadir = 0;
	}
    }

  return datadir;
}

int main(int argc, char *argv[])
{
  char *datadir = setup_datadir(argc, argv);

  if(!datadir)
    exit(1);

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
  free(datadir);

#ifndef NO_SDL_MIXER
  Mix_CloseAudio();
#endif

  SDL_Quit();
}
