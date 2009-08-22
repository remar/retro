#ifndef GOLD_DRONE_H
#define GOLD_DRONE_H

#include "Drone.h"

class GoldDrone : public Drone
{
 public:
  GoldDrone(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper);
  virtual void update(Field *field, Hero *hero);
};

#endif
