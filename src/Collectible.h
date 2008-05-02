#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "Object.h"

class Collectible : public Object
{
 public:
  Collectible(remar2d *gfx, char *sprite, SoundManager *sfx);
  virtual void update();
  virtual void hit();
};

#endif
