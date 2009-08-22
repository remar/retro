#ifndef MENU_H
#define MENU_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "ScoreKeeper.h"
#include "GameMode.h"
#include "Counter.h"

class Menu : public GameMode
{
 public:
  Menu(remar2d *gfx, SoundManager *sfx, Input *input,
       ScoreKeeper *scoreKeeper);
  ~Menu();
  Mode update();

 private:
  void drawBackground();
  void drawPattern(int *arr, int xOffset ,int yOffset, int width, int height);
  void increaseLevel();
  void decreaseLevel();
  void increaseSkillLevel();
  
  char *mainTiles;

  Counter *levelCounter;
  int level;

  Counter *skillCounter;
  int skill;

  int nextTimer;

  /* Strings */
  int remar_games_2009;
  int enter_to_start;
  int stage;
  int skillLevel;
  int score;
  int topScore;
};

#endif
