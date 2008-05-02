#include "Spawner.h"

// Spawner::Spawner(Nest **nest, Fuzz **fuzz, int type, int maxNumber)
Spawner::Spawner(list<Object *> *objects, int type, int maxNumber)
  : numberOfNests(0)
{
  // this->nest = nest;
  // this->fuzz = fuzz;
  // this->enemies = enemies;
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

//       if(nest[i])
// 	numberOfNests++;
  list<Object *>::iterator it;
  for(it = objects->begin();it != objects->end();it++)
    {
      if(Nest *nest = dynamic_cast<Nest *>(*it))
	{
	  printf("NEST!\n");
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

//   for(it = nests.begin(), i=0;it != nests.end();it++, i++)
//     {
//       if(nestCountDown[i] == 1)
// 	{
// 	  (*it)->blink(false);
// 	  (*it)->spawn();
// 	}
//       if(nestCountDown[i] > 0)
// 	nestCountDown[i]--;
//     }

//   for(int i = 0;i < numberOfNests;i++)
//     {
//       if(nestCountDown[i] == 1)
// 	{
// 	  nest[i]->blink(false);
// 	  nest[i]->spawn(findEmptyFuzz());
// 	}
//       if(nestCountDown[i] > 0)
// 	nestCountDown[i]--;
//     }

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
	  if((*it)->isSpawning() == false) //nestCountDown[n] == 0)
	    {
// 	      nest[n]->blink(true);
// 	      nestCountDown[n] = 60;
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

// Fuzz **
// Spawner::findEmptyFuzz()
// {
//   for(int i = 0;i < 8;i++)
//     if(fuzz[i] == 0)
//       return &fuzz[i];

//   /* This should never happen!! */
//   printf("BUG: No empty fuzz holder found!\n");
//   return 0;
// }
