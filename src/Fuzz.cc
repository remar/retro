#include "Fuzz.h"

Fuzz::Fuzz(remar2d *gfx)
  : Object(gfx, "fuzz"), moveDirection(NONE)
{
  setAnimation("roll left");

  setBoundingBox(24, 24, 0, 0);
}

void
Fuzz::rollLeft()
{
  if(rollDirection != LEFT)
    {
      setAnimation("roll left");
      rollDirection = LEFT;
      stunned = false;
    }
}

void
Fuzz::rollRight()
{
  if(rollDirection != RIGHT)
    {
      setAnimation("roll right");
      rollDirection = RIGHT;
      stunned = false;
    }
}

void
Fuzz::rollRandom()
{
  if(rand()%2)
    rollRight();
  else
    rollLeft();
}

void
Fuzz::stun()
{
  if(rollDirection != NONE)
    {
      setAnimation("stunned");
      stunned = true;
      rollDirection = NONE;
      blinkTime = SDL_GetTicks() + 2000;
    }

  if(blinkTime < SDL_GetTicks())
    {
      setAnimation("blink");
      blinkTime = 2000000000;
    }
}

void
Fuzz::blink()
{
  if(rollDirection != NONE)
    {
      setAnimation("blink");
      rollDirection = NONE;
    }
}

// void
// Fuzz::pause(bool on)
// {
//   if(on)
    
// }
