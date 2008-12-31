#ifndef BONUS_SCORE_SCREEN_H
#define BONUS_SCORE_SCREEN_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "ScoreKeeper.h"
#include "GameMode.h"
#include "Counter.h"

class BonusScoreScreen : public GameMode
{
 public:
  BonusScoreScreen(remar2d *gfx, SoundManager *sfx, Input *input,
		   ScoreKeeper *scoreKeeper);
  ~BonusScoreScreen();
  Mode update();

 private:
  void drawBackground();

  enum State { TIME, COINS, PERFECT, DONE };
  enum SubState { SHOW, TICK, TICKDELAY };

  State state;
  SubState subState;

  int delayTimer;
  int tickDelayTimer;

  char *mainTiles;

  int nextTimer;

  /* Strings */
  int top;
  int score;
  int stage;

  Counter *topScoreCounter;
  Counter *scoreCounter;
  Counter *level;

  int clock;
  Counter *time;
  int scorePerTime;

  int coinSprite;
  Counter *coins;
  int coinsCounted;
  int scorePerCoin;

  int perfect;
  int twothousand;

/*   int fuzzSprite; */
/*   Counter *fuzzes; */
/*   int fuzzesCounted; */
/*   int scorePerFuzz; */

/*   int droneSprite; */
/*   Counter *drones; */
/*   int dronesCounted; */
/*   int scorePerDrone; */

/*   int viperSprite; */
/*   Counter *vipers; */
/*   int vipersCounted; */
/*   int scorePerViper; */

/*   int hunterSprite; */
/*   Counter *hunters; */
/*   int huntersCounted; */
/*   int scorePerHunter; */
};

#endif
