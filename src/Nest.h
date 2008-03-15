#ifndef NEST_H
#define NEST_H

#include <remar2d.h>
#include "Object.h"
#include "Fuzz.h"

class Nest : public Object
{
 public:
  Nest(remar2d *gfx);
  void blink(bool on);
  void spawn(Fuzz **fuzz);
};

#endif
