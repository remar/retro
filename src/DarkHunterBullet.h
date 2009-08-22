#ifndef DARK_HUNTER_BULLET_H
#define DARK_HUNTER_BULLET_H

#include "HunterBullet.h"

class DarkHunterBullet : public HunterBullet
{
 public:
  DarkHunterBullet(remar2d *gfx, SoundManager *sfx,
		   list<Enemy *> *enemies, list<Object *> *objects);
  virtual void update(Field *field, Hero *hero);

 private:
  list<Enemy *> *enemies;

 protected:
  virtual void blockHit(Field *field, int blockX, int blockY);
};

#endif
