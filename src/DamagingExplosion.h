#ifndef DAMAGING_EXPLOSION_H
#define DAMAGING_EXPLOSION_H

#include <remar2d.h>
#include "Object.h"

class DamagingExplosion : public Object
{
 public:
  DamagingExplosion(remar2d *gfx, SoundManager *sfx, int x, int y);
  bool checkCollision();

 private:
  bool checked;
};

#endif
