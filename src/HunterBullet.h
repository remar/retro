#ifndef HUNTER_BULLET_H
#define HUNTER_BULLET_H

#include "Enemy.h"

class HunterBullet : public Enemy
{
 public:
  HunterBullet(remar2d *gfx, SoundManager *sfx, list<Object *> *objects);
  void moveLeft();
  void moveRight();
  void update(Field *field, Hero *hero);
  
 private:
  enum Direction {LEFT, RIGHT};
  Direction direction;

  list<Object *> *objects;
};

#endif
