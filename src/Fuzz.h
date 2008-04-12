#ifndef FUZZ_H
#define FUZZ_H

#include <remar2d.h>
#include "Object.h"
#include "Field.h"
#include "Hero.h"

class Fuzz : public Object
{
 public:
  enum Direction {NONE, LEFT, RIGHT, UP, DOWN};

  Fuzz(remar2d *gfx, SoundManager *sfx);
  void update(Field *field, Hero *hero);
  void rollLeft();
  void rollRight();
  void rollRandom();
  void stun();
  void blink();
  void die();
  Direction getRollDir() { return rollDirection; }
  Direction getMoveDir() { return moveDirection; }
  void setMoveDir(Direction dir) { moveDirection = dir; }
  bool isStunned();

 private:
  void attach(int posX1, int posX2, int posY1, int posY2, Direction direction);
  void attachNone();

  Direction moveDirection;
  Direction rollDirection;
  Direction lastAttachDirection;

  bool stunned;
  bool falling;
  unsigned int blinkTime;
  int fallDistance;
  bool fastFuzz;
  bool dead;
  bool onSpikes;

  int FALL_LIMIT;
  int SPIKES_LEVEL;

  /* Keep track of the last block(s) that we we're attached to */
  int last1_x, last1_y;
  int last2_x, last2_y;

  int deathTimer;
  int stunTimer;
};

#endif
