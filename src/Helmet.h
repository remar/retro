#ifndef HELMET_H
#define HELMET_H

#include <remar2d.h>
#include "Object.h"

class Helmet : public Object
{
 public:
  Helmet(remar2d *gfx, SoundManager *sfx);
};

#endif
