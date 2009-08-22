#ifndef DARK_EXPLOSION_H
#define DARK_EXPLOSION_H

#include "Enemy.h"

class DarkExplosion : public Enemy
{
 public:
  DarkExplosion(remar2d *gfx, SoundManager *sfx, int x, int y);
  virtual void update(Field *field, Hero *hero);

 private:
  bool firstUpdate;
};

#endif
