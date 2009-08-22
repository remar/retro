#ifndef DIRE_SPACE_VIPER_H
#define DIRE_SPACE_VIPER_H

#include "SpaceViper.h"

class DireSpaceViper : public SpaceViper
{
 public:
  DireSpaceViper(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper,
		 list<Enemy *> *enemies, int length);
  ~DireSpaceViper();
  void update(Field *field, Hero *hero);
};

#endif
