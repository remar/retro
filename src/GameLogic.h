#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "ScoreKeeper.h"
#include "GameMode.h"

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

  /* Game mode */
  GameMode *gameMode;
  Mode mode;

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
