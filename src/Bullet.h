#ifndef BULLET_H
#define BULLET_H

#include <remar2d.h>
#include "Object.h"

class Bullet : public Object
{
 public:
  Bullet(remar2d *gfx, SoundManager *sfx);
  void moveLeft();
  void moveRight();
  void update();

  enum Direction {LEFT, RIGHT};
  Direction getDirection();


 protected:
  Direction direction;
};

#endif
