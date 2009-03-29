#ifndef SPACE_VIPER_H
#define SPACE_VIPER_H

#include "Enemy.h"
#include "SpaceViperBody.h"

class SpaceViper : public Enemy
{
 public:
  SpaceViper(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper,
	     list<Enemy *> *enemies, int length);
  ~SpaceViper();
  void update(Field *field, Hero *hero);
  bool hit();

 private:
  enum Direction {NONE, LEFT, RIGHT, UP, DOWN};

  void die();
  void updateAnimation();
  bool tailAtPosition(int blockX, int blockY);
  void getBlockAtDirection(Direction direction, int *blockX, int *blockY);
  
  Object *block;

  list<Enemy *> *enemies;

  /* Head sprite */
  int head;

  /* Body objects */
  list<SpaceViperBody *> body;

  Direction moveDirection;
  int hitPoints;
  bool dead;
  int length;

  bool moveThisFrame;

  /* Keep track of how long we've moved, randomize direction every 32 pixels */
  int moved;
};

#endif
