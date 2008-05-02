#include "Counter.h"

Counter::Counter(remar2d *gfx, int length)
{
  this->gfx = gfx;
  this->length = length;

  sprite_instances = new int[length];

  for(int i = 0;i < length;i++)
    {
      sprite_instances[i] = gfx->createSpriteInstance("numbers");
      gfx->setAnimation(sprite_instances[i], "0");
      gfx->showSprite(sprite_instances[i], true);
    }

  setPosition(0, 0);
}

Counter::~Counter()
{
  for(int i = 0;i < length;i++)
    gfx->removeSpriteInstance(sprite_instances[i]);

  delete [] sprite_instances;
}

void
Counter::setPosition(int x, int y)
{
  for(int i = 0;i < length;i++)
    {
      gfx->moveSpriteAbs(sprite_instances[i], x+i*16, y);
    }
}

int exp10(int n)
{
  int retval = 1;

  for(int i = 0;i < n;i++)
    {
      retval *= 10;
    }

  return retval;
}

void
Counter::setCounter(int v)
{
  if(v == value || v < 0 || v >= exp10(length))
    return;

  value = v;

  int temp = value;
  char *nums[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

  for(int i = length-1, j=0;i >= 0;i--,j++)
    {
      int num = temp / exp10(i);
      gfx->setAnimation(sprite_instances[j], nums[num]);
      temp %= exp10(i);
    }
}
