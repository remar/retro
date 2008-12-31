#ifndef SPACE_VIPER_H
#define SPACE_VIPER_H

#include "Enemy.h"
#include "SpaceViperBody.h"

class SpaceViper : public Enemy
{
 public:
  SpaceViper(remar2d *gfx, SoundManager *sfx, list<Enemy *> *enemies);
  ~SpaceViper();
  void update(Field *field, Hero *hero);
  bool hit();

 private:
  void die();
  void updateAnimation();
  bool tailAtPosition(int blockX, int blockY);
  
  list<Enemy *> *enemies;

  /* Head sprite */
  int head;

  /* Body objects */
  list<SpaceViperBody *> body;

  enum Direction {NONE, LEFT, RIGHT, UP, DOWN};

  Direction moveDirection;
  int hitPoints;
  bool dead;
  int length;

  bool moveThisFrame;

  /* Keep track of how long we've moved, randomize direction every 32 pixels */
  int moved;
};

#endif
