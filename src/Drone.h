#ifndef DRONE_H
#define DRONE_H

#include "Enemy.h"

class Drone : public Enemy
{
 public:
  Drone(remar2d *gfx, SoundManager *sfx);
  void update(Field *field, Hero *hero);
  bool hit();

 private:
  void die();

  enum Direction {NONE, LEFT, RIGHT, UP, DOWN};

  Direction moveDirection;
  int moved;
  bool willDig;
  bool moveThisUpdate;
  int hitPoints;

  bool dead;
  int deathTimer;
};

#endif
