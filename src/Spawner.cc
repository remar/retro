#include "Spawner.h"

Spawner::Spawner(Nest **nest, Fuzz **fuzz, int type, int maxNumber)
  : numberOfNests(0)
{
  this->nest = nest;
  this->fuzz = fuzz;
  this->type = type;
  this->maxNumber = maxNumber;

  startSpawn = true;
  inQueue = maxNumber;
  countDown = 60; /* 60 frames to next spawn */

  for(int i = 0;i < 8;i++)
    {
      nestCountDown[i] = 0;

      if(nest[i])
	numberOfNests++;
    }
}

void
Spawner::update()
{
  for(int i = 0;i < numberOfNests;i++)
    {
      if(nestCountDown[i] == 1)
	{
	  nest[i]->blink(false);
	  nest[i]->spawn(findEmptyFuzz());
	}
      if(nestCountDown[i] > 0)
	nestCountDown[i]--;
    }

  if(inQueue == 0)
    return;

  countDown--;
  if(countDown <= 0)
    {
      bool nestFound = false;

      while(nestFound == false)
	{
	  int n = rand() % numberOfNests;
	  if(nest[n] && nestCountDown[n] == 0)
	    {
	      nest[n]->blink(true);
	      nestCountDown[n] = 60;
	      nestFound = true;
	    }
	}

      inQueue--;

      if(startSpawn && inQueue == 0)
	startSpawn = false;

      if(startSpawn)
	countDown = 60;
      else
	countDown = 60 * 8; /* 8 seconds */
    }
}

void
Spawner::addFuzz()
{
  inQueue++;
}

Fuzz **
Spawner::findEmptyFuzz()
{
  for(int i = 0;i < 8;i++)
    if(fuzz[i] == 0)
      return &fuzz[i];

  /* This should never happen!! */
  printf("BUG: No empty fuzz holder found!\n");
  return 0;
}
