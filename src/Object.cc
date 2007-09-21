#include "Object.h"

Object::Object(remar2d *gfx, char *sprite)
{
  this->gfx = gfx;
  sprite_instance = gfx->createSpriteInstance(sprite);
}

void
Object::moveAbs(float x, float y)
{
  pos_x = x;
  pos_y = y;

  gfx->moveSpriteAbs(sprite_instance, getX(), getY());
}


void
Object::moveRel(float x, float y)
{
  pos_x += x;
  pos_y += y;

  gfx->moveSpriteAbs(sprite_instance, getX(), getY());
}

void
Object::setVisible(bool visible)
{
  gfx->showSprite(sprite_instance, visible);
}

void
Object::setAnimation(char *animation)
{
  gfx->setAnimation(sprite_instance, animation); 
}

void
Object::setBoundingBox(int width, int height, int offset_x, int offset_y)
{
  bounding_box.w = width - 1;
  bounding_box.h = height - 1;
  bounding_box.x = offset_x;
  bounding_box.y = offset_y;
}

int
Object::getX()
{
  float q = pos_x - (int)pos_x;
  if(q > 0.5)
    return (int)pos_x+1;
  else
    return (int)pos_x;
}

int
Object::getY()
{
  float q = pos_y - (int)pos_y;
  if(q > 0.5)
    return (int)pos_y+1;
  else
    return (int)pos_y;
}

SDL_Rect *
Object::getBoundingBox()
{
  return &bounding_box;
}
