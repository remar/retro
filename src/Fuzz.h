#ifndef FUZZ_H
#define FUZZ_H

#include <remar2d.h>
#include "Object.h"

class Fuzz : public Object
{
 public:
  enum MoveDir {NONE, LEFT, RIGHT, UP, DOWN};

  Fuzz(remar2d *gfx);
  void rollLeft();
  void rollRight();
  void rollRandom();
  void stun();
  void blink();
  MoveDir getRollDir() { return rollDirection; }
  MoveDir getMoveDir() { return moveDirection; }
  void setMoveDir(MoveDir dir) { moveDirection = dir; }

 private:
  MoveDir moveDirection;
  MoveDir rollDirection;
  bool stunned;
  unsigned int blinkTime;
};

#endif
