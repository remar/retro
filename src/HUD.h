#ifndef HUD_H
#define HUD_H

#include <remar2d.h>
#include "ScoreKeeper.h"
#include "Counter.h"

class HUD
{
 public:
  enum Display { SCORE, TOP, TIME, LIVES, STAGE, SKILL, RELOAD, BULLET };

  HUD(remar2d *gfx, ScoreKeeper *scoreKeeper);
  ~HUD();
  void update();  
  void setValue(Display display, int value);

 private:
  void setAmmo(int value);
  void setReload(int value);

  remar2d *gfx;
  Counter *counters[6];
  ScoreKeeper *scoreKeeper;

  int hudSprite;

  int ammoSprites[8];
  int reloadSprites[8];
};

#endif
