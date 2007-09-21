/*
 *  No Friction - roll-a-ball-puzzle-game
 *  Copyright (C) 2005  Andreas Remar
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Input.h"

Input::Input()
{
  Input(500);
}

Input::Input(int _timeout)
  : quitEvent(false)
{
  setTimeout(_timeout);
}

void
Input::setTimeout(int _timeout)
{
  timeout = _timeout;
}

void
Input::getInput()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_KEYDOWN:
	  keys[event.key.keysym.sym].pressed = 1;
	  keys[event.key.keysym.sym].checked = 0;
	  keys[event.key.keysym.sym].timeout = 0;
	  break;

	case SDL_KEYUP:
	  keys[event.key.keysym.sym].pressed = 0;
	  keys[event.key.keysym.sym].checked = 0;
	  keys[event.key.keysym.sym].timeout = 0;
	  break;

	case SDL_QUIT:
	  quitEvent = true;
	  break;
	}
    }

  /* update timeouts */
  /* TODO: do this more efficient by storing pressed keys in a list */
//   for(int i = 0;i < SDLK_LAST;i++)
//     {
//       if(keys[i].timeout < SDL_GetTicks())
// 	{
// 	  keys[i].checked = 0;
// 	  keys[i].timeout = SDL_GetTicks() + timeout;
// 	  printf("%d timeout out\n", i);
// 	}
//     }
}

bool
Input::pressed(int key)
{
  if(keys[key].pressed == 1 && keys[key].checked == 0)
    {
      keys[key].checked = 1;
      return true;
    }

  return false;
}

bool
Input::released(int key)
{
  if(keys[key].pressed == 0 && keys[key].checked == 0)
    {
      keys[key].checked = 1;
      return true;
    }

  return false;
}

bool
Input::held(int key)
{
  return keys[key].pressed;
}

bool
Input::quit()
{
  return quitEvent;
}
