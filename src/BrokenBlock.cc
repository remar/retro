#include "BrokenBlock.h"

BrokenBlock::BrokenBlock(int x, int y)
{
  this->x = x;
  this->y = y;

  respawnTimer = 8*60; /* 8 seconds respawn time */
}

void
BrokenBlock::update()
{
  if(respawnTimer > 0)
    respawnTimer--;
}

bool
BrokenBlock::respawn()
{
  return respawnTimer == 0;
}

int
BrokenBlock::getX()
{
  return x;
}

int
BrokenBlock::getY()
{
  return y;
}

void
BrokenBlock::resetRespawnTimer(int value)
{
  respawnTimer = value;
}
