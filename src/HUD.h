#ifndef HUD_H
#define HUD_H

#include <remar2d.h>
#include "ScoreKeeper.h"
#include "Counter.h"

class HUD
{
 public:
  enum Display { SCORE, TOP, TIME, LIVES, STAGE, SKILL, BULLET, RELOAD };

  HUD(remar2d *gfx, ScoreKeeper *scoreKeeper);
  void update();  
  void setValue(Display display, int value);

 private:
  Counter *counters[6];
  ScoreKeeper *scoreKeeper;

  int hudSprite;

/*   Counter *score; */
/*   Counter *topScore; */
/*   Counter *time; */
/*   Counter *lives; */
/*   Counter *level; */
/*   Counter *skill; */
};

#endif
