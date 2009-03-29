#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "Bullet.h"

class LaserBeam : public Bullet
{
 public:
  LaserBeam(remar2d *gfx, SoundManager *sfx);
};

#endif // LASER_BEAM_H
