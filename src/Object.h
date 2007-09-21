#ifndef OBJECT_H
#define OBJECT_H

#include "SDL.h"
#include <remar2d.h>

/* Animated object in the game */
class Object
{
 public:
  Object(remar2d *gfx, char *sprite);
  void moveAbs(float x, float y);
  void moveRel(float x, float y);
  void setVisible(bool visible);
  void setAnimation(char *animation);
  void setBoundingBox(int width, int height, int offset_x, int offset_y);
  int getX();
  int getY();
  SDL_Rect *getBoundingBox();

  /* Update position and actions in the level. */
  void update(int delta);

 protected:
  remar2d *gfx;
  int sprite_instance;
  float pos_x, pos_y;
  SDL_Rect bounding_box;
};

#endif
