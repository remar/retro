#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Object.h"

class Explosion : public Object
{
 public:
  Explosion(remar2d *gfx, SoundManager *sfx, int x, int y);
  void update();

 private:
  int deathTimer;
};

#endif
