#ifndef COUNTER_H
#define COUNTER_H

#include <remar2d.h>

class Counter
{
 public:
  Counter(remar2d *gfx, int length);
  ~Counter();
  void setPosition(int x, int y);
  void setCounter(int v);

 private:
  remar2d *gfx;
  int *sprite_instances;
  int length;
  int value;
};

#endif
