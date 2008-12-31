#include "Spawner.h"

Spawner::Spawner(list<Object *> *objects, int type, int maxNumber)
  : numberOfNests(0)
{
  this->objects = objects;
  this->type = type;
  this->maxNumber = maxNumber;

  startSpawn = true;
  inQueue = maxNumber;
  countDown = 60; /* 60 frames to next spawn */

  for(int i = 0;i < 8;i++)
    {
      nestCountDown[i] = 0;
    }

  list<Object *>::iterator it;
  for(it = objects->begin();it != objects->end();it++)
    {
      if(Nest *nest = dynamic_cast<Nest *>(*it))
	{
	  nests.push_back(nest);
	  numberOfNests++;
	}
    }
}

void
Spawner::update()
{
  list<Nest *>::iterator it;
  int i;

  if(inQueue == 0)
    return;

  countDown--;
  if(countDown <= 0)
    {
      bool nestFound = false;

      while(nestFound == false)
	{
	  int n = rand() % numberOfNests;

	  for(it = nests.begin(), i = 0;it != nests.end() && i != n;it++, i++);
	  if((*it)->isSpawning() == false)
	    {
	      (*it)->spawn();
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
