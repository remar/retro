#ifndef SMOKE_H
#define SMOKE_H

#include "Object.h"

class Smoke : public Object
{
 public:
  Smoke(remar2d *gfx, SoundManager *sfx, int x, int y);
  void update();

 private:
  int deathTimer;
};

#endif
