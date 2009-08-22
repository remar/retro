#ifndef DARK_BOUNTY_HUNTER_H
#define DARK_BOUNTY_HUNTER_H

#include "BountyHunter.h"

class DarkBountyHunter : public BountyHunter
{
 public:
  DarkBountyHunter(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper,
		   list<Enemy *> *enemies, list<Object *> *objects);
  void update(Field *field, Hero *hero);
  void fire();
};

#endif
