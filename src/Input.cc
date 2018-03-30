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

#include "Input.h"

Input::Input()
{
  setTimeout(500);
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
  resetActionKeys();
}

void
Input::getInput()
{
  SDL_Event event;
  key_type key;
  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_KEYDOWN:
	  if(event.key.repeat == 0) {
	    key.pressed = 1;
	    key.checked = 0;
	    key.timeout = 0;
	    keys[event.key.keysym.sym] = key;
	  }
	  break;

	case SDL_KEYUP:
	  key.pressed = 0;
	  key.checked = 0;
	  key.timeout = 0;
	  keys[event.key.keysym.sym] = key;
	  break;

	case SDL_JOYBUTTONDOWN:
	  printf("Joystick: %d, button: %d\n",
		 event.jbutton.which,
		 event.jbutton.button);
	  break;

	case SDL_QUIT:
	  quitEvent = true;
	  break;
	}
    }
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

int
Input::getKeyPressed()
{
  for(auto const& x : keys) {
    if(x.second.pressed) {
      return x.first;
    }
  }

  return 0;
}

bool
Input::defineActionKey(ACTION action, int key)
{
  for(int i = 0;i < 4;i++)
    {
      if(actionKeys[i] == key && (ACTION)i != action)
	{
	  /* Key already in use */
	  return false;
	}
    }

  actionKeys[action] = key;

  return true;
}

void
Input::resetActionKeys()
{
  actionKeys[0] = 0;
  actionKeys[1] = 0;
  actionKeys[2] = 0;
  actionKeys[3] = 0;
}

bool
Input::actionPressed(ACTION action)
{
  return pressed(actionKeys[action]);
}

bool
Input::actionReleased(ACTION action)
{
  return released(actionKeys[action]);
}

bool
Input::actionHeld(ACTION action)
{
  return held(actionKeys[action]);
}

bool
Input::quit()
{
  return quitEvent;
}
