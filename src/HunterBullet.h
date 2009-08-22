#ifndef HUNTER_BULLET_H
#define HUNTER_BULLET_H

#include "Enemy.h"

class HunterBullet : public Enemy
{
 public:
  HunterBullet(remar2d *gfx, SoundManager *sfx, list<Object *> *objects);
  void moveLeft();
  void moveRight();
  virtual void update(Field *field, Hero *hero);
  
 private:
  enum Direction {LEFT, RIGHT};
  Direction direction;

  list<Object *> *objects;

 protected:
  virtual void blockHit(Field *field, int blockX, int blockY);
};

#endif
