#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

typedef struct
{
  int pressed; /* key pressed? */
  int checked; /* key checked? */
  int timeout; /* when to repeat key */
} key_type;

class Input
{
 private:
  key_type keys[SDLK_LAST];
  int timeout;
  bool quitEvent;

 public:
  Input();
  Input(int _timeout);

  /* Set a new timeout period to allow configurability. */
  void setTimeout(int _timeout);

  /* Read input states (only keyboard as of now) */
  void getInput();

  /* Checks if key has been pressed since last update (only returns
     'true' once for each keypress) */
  bool pressed(int key);

  /* Checks if key has been released since last update (only returns
     'true' once for each key release) */
  bool released(int key);

  /* Check if key is held down (returns 'true' every time the key is
     pressed) */
  bool held(int key);

  /* Return 'true' if a SDL_QUIT event has been seen.  This occurs
     when the user clicks the X in the corner of the window. */
  bool quit();
};

#endif
