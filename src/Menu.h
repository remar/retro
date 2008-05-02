#ifndef MENU_H
#define MENU_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "ScoreKeeper.h"
#include "GameMode.h"
#include "Counter.h"

class Menu
{
 public:
  Menu(remar2d *gfx, SoundManager *sfx, Input *input,
       ScoreKeeper *scoreKeeper);
  ~Menu();
  GameMode update();

 private:
  void drawBackground();
  void increaseLevel();
  void decreaseLevel();

  remar2d *gfx;
  SoundManager *sfx;
  Input *input;
  ScoreKeeper *scoreKeeper;
  
  char *mainTiles;

  Counter *levelCounter;
  int level;

  int nextTimer;
};

#endif
