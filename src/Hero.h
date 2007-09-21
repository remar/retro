#ifndef HERO_H
#define HERO_H

#include <remar2d.h>
#include "Object.h"

class Hero : public Object
{
 public:
  Hero(remar2d *gfx);
  void moveRel(float xDir, float yDir, bool onGround);
  void stop();
  void jump(bool on);
  bool jumps(int decrease);

 private:
  int direction;
  int jumpCounter;
  bool jumping;

  enum Animation
    {
      FLYING,
      WALKING,
      STANDING
    };

  Animation currentAnimation;
};

#endif
