#ifndef HERO_H
#define HERO_H

#include <remar2d.h>
#include "Object.h"
#include "Bullet.h"

class Hero : public Object
{
 public:
  Hero(remar2d *gfx, SoundManager *sfx);
  ~Hero();
  void moveRel(float xDir, float yDir); //, bool onGround);
  void stop();
  void jump(bool on);
  bool jumps(int decrease);
  void shoot(int *bullets, Bullet **bullet);
  void die();
  void update();
  bool isBlinking() { return blinking; }
  bool isDead() { return dead; }

 private:
  void updateAnimation(float xDir, float yDir);
  void setDirection(float dir);

  enum Direction
    {
      LEFT,
      RIGHT
    };

  Direction direction;
  int jumpCounter;
  bool jumping;
  bool allowJumping;
  bool falling;

  bool blinking;
  bool dead;

  /* The flame sprite (displayed when jumping) */
  int flame;
  bool flameShown;
  Direction flameDirection;

  float oldXDir;
  float oldYDir;

  enum Animation
    {
      FLYING,
      WALKING,
      STANDING
    };

  Animation currentAnimation;
  
  /* channel that the walk sound is played on */
  int channel;

  int deathTimer;
};

#endif
