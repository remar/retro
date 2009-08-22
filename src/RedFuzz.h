#ifndef RED_FUZZ_H
#define RED_FUZZ_H

#include "Fuzz.h"

class RedFuzz : public Fuzz
{
 public:
  RedFuzz(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper);
  void update(Field *field, Hero *hero);

 private:
  int directionTimeout;
};

#endif
