#include "BulletHandler.h"

BulletHandler::BulletHandler(HUD *hud)
{
  this->hud = hud;

  ammo = 8;
  refillCounter = 0;
  refillCounterTimer = 0;
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

	  if(refillCounter == 8)
	    {
	      ammo++;

	      hud->setValue(HUD::BULLET, ammo);

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

  return true;
}
