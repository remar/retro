#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "Hero.h"
#include "Fuzz.h"
#include "Level.h"
#include "ScoreKeeper.h"
#include "GameMode.h"
#include "Menu.h"
#include "ScoreScreen.h"

class GameLogic
{
 public:
  GameLogic(Input *i, remar2d *gfx, SoundManager *sfx);
  ~GameLogic();
  void update();
  bool quit();

 private:
  Input *input;
  remar2d *graphics;
  SoundManager *sound;

  ScoreKeeper *scoreKeeper;

  /* Game modes */
  Menu *menu;
  Level *level;
  ScoreScreen *scoreScreen;

  GameMode *gameMode;

  Mode mode;

  int oldTime;

  /* Fixed interval time-based animation */
  static const int maximumFrameRate = 60;
  static const int minimumFrameRate = 15;
  static const float updateInterval = 1.0 / maximumFrameRate;
  static const float maxCyclesPerFrame = maximumFrameRate / minimumFrameRate;

  float lastFrameTime;
  float cyclesLeftOver;

  bool quitGame;

  bool fullScreen;
};

#endif
