#ifndef SPACE_VIPER_BODY_H
#define SPACE_VIPER_BODY_H

#include "Enemy.h"

class SpaceViperBody : public Enemy
{
 public:
  SpaceViperBody(remar2d *gfx, SoundManager *sfx, bool bright);
  void update(Field *field, Hero *hero);
  bool hit();
  void splat();

 private:
};

#endif // SPACE_VIPER_BODY_H
