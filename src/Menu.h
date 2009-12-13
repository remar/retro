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
  enum SUBMODE {NORMAL, REDEFINE_LEFT, REDEFINE_RIGHT, REDEFINE_FIRE,
		REDEFINE_JUMP};

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
  int space_to_set_keys;
  int stage;
  int skillLevel;
  int score;
  int topScore;

  int actionStrings[4];

  SUBMODE subMode;

  int blinkDelay;

  int keyConfig[4];
};

#endif
