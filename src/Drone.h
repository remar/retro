#ifndef DRONE_H
#define DRONE_H

#include "Enemy.h"

class Drone : public Enemy
{
 public:
  Drone(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper);
  virtual void update(Field *field, Hero *hero);
  bool hit();

 protected:
  bool aimAtHero;

 private:
  enum Direction {NONE, LEFT, RIGHT, UP, DOWN};

  void die();
  void getBlockInDirection(Direction dir, int *blockX, int *blockY);

  Direction moveDirection;
  int moved;
  bool willDig;
  bool moveThisUpdate;
  int hitPoints;

  bool dead;
  int deathTimer;

};

#endif
