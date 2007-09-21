#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <remar2d.h>
#include "Input.h"
#include "Hero.h"
#include "Fuzz.h"
#include "Level.h"

class GameLogic
{
 public:
  GameLogic(Input *i, remar2d *gfx);
  ~GameLogic();
  void update();
  bool quit();

 private:
  Input *input;
  remar2d *graphics;

  Hero *hero;
  Fuzz *fuzz, *fuzz2;

  Level *level;

  enum GameMode
    {
      MENU,
      GAME,
      QUIT
    };

  GameMode mode;

  int oldTime;

  /* TODO: Classes to handle levels, monsters, player and STUFF! */
};

#endif
