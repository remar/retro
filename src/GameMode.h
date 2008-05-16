#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "ScoreKeeper.h"

enum Mode
  {
    MENU,
    GAME,
    SCORE,
    QUIT
  };

class GameMode
{
 public:
  GameMode(remar2d *gfx, SoundManager *sfx, Input *input,
	ScoreKeeper *scoreKeeper);
  virtual ~GameMode();
  virtual Mode update();

 protected:
  remar2d *gfx;
  SoundManager *sfx;
  Input *input;
  ScoreKeeper *scoreKeeper;
};

#endif
