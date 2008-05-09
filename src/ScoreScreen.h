#ifndef SCORE_SCREEN_H
#define SCORE_SCREEN_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "ScoreKeeper.h"
#include "GameMode.h"
#include "Counter.h"
#include "Fuzz.h"
#include "Drone.h"

class ScoreScreen
{
 public:
  ScoreScreen(remar2d *gfx, SoundManager *sfx, Input *input,
	      ScoreKeeper *scoreKeeper);
  ~ScoreScreen();
  GameMode update();

 private:
  void drawBackground();

  remar2d *gfx;
  SoundManager *sfx;
  Input *input;
  ScoreKeeper *scoreKeeper;
  
  char *mainTiles;

  int frameCounter;
  
  int nextTimer;

  /* Strings */
  int top;
  int score;
  int stage;

  int scorePerTime;
  int scorePerCoin;
  int scorePerFuzz;
  int scorePerDrone;
  int scorePerViper;
  int scorePerHunter;

  Counter *topScoreCounter;
  Counter *scoreCounter;

  Counter *level;

  int clock;
  Counter *time;

  Counter *coins;
  Counter *fuzzes;
  Counter *drones;
  Counter *vipers;
  Counter *hunters;

  Fuzz *fuzz;
  Drone *drone;
};

#endif
