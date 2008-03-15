#ifndef BULLET_H
#define BULLET_H

#include <remar2d.h>
#include "Object.h"

class Bullet : public Object
{
 public:
  Bullet(remar2d *gfx);
  void moveLeft();
  void moveRight();
  void update();

 private:
  enum Direction {LEFT, RIGHT};
  Direction direction;
};

#endif
