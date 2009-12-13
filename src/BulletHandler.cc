#include "BulletHandler.h"

BulletHandler::BulletHandler(HUD *hud)
  : hud(hud)
{
  reset();
//   ammo = 8;
//   refillCounter = 0;
//   refillCounterTimer = 0;
}

void
BulletHandler::update()
{
  if(refillCounterTimer > 0)
    {
      --refillCounterTimer;
      if(refillCounterTimer == 0)
	{
	  refillCounter++;

	  if(refillCounter == 9)
	    {
	      ammo++;

	      hud->setValue(HUD::BULLET, ammo);
	      hero->showNote(ammo);

	      refillCounter = 0;
	    }
	  else
	    {
	      refillCounterTimer = 60;
	    }

	  hud->setValue(HUD::RELOAD, refillCounter);
	}
    }

  if(ammo < 8 && refillCounter == 0 && refillCounterTimer == 0)
    {
      /* Start refilling */
      refillCounterTimer = 60;
    }
}

bool
BulletHandler::fire()
{
  if(ammo == 0)
    return false;

  ammo--;

  hud->setValue(HUD::BULLET, ammo);

  hero->showNote(ammo);

  return true;
}

void
BulletHandler::reset()
{
  ammo = 8;
  refillCounter = 0;
  refillCounterTimer = 0;

  hud->setValue(HUD::BULLET, ammo);
  hud->setValue(HUD::RELOAD, refillCounter);
}

void
BulletHandler::addOne()
{
  if(ammo == 0)
    {
      ammo++;
      hud->setValue(HUD::BULLET, ammo);
      hero->showNote(ammo);
    }
}
