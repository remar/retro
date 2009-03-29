#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"
#include "ScoreKeeper.h"
#include "Field.h"
#include "Hero.h"

class Enemy : public Object
{
 public:
  Enemy(remar2d *gfx, char *sprite, SoundManager *sfx,
	ScoreKeeper *scoreKeeper);
  ~Enemy();
  virtual void update(Field *field, Hero *hero);

  // Hit by Captain Goods blaster! Return true if bullet should be removed.
  virtual bool hit();

 protected:
  ScoreKeeper *scoreKeeper;
};

#endif
