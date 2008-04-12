#ifndef MENU_H
#define MENU_H

#include <remar2d.h>
#include "Input.h"
#include "ScoreKeeper.h"

class Menu
{
 public:
  Menu(remar2d *gfx, Input *input, ScoreKeeper *scoreKeeper);
  ~Menu();
  GameMode update();

 private:
  remar2d *gfx;
  Input *input;
  ScoreKeeper *scoreKeeper;
};

#endif
