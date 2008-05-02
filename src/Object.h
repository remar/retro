#ifndef OBJECT_H
#define OBJECT_H

#include "SDL.h"
#include <remar2d.h>
#include "SoundManager.h"

/* Animated object in the game */
class Object
{
 public:
  Object(remar2d *gfx, char *sprite, SoundManager *sfx);
  virtual ~Object();
  void moveAbs(float x, float y);
  void moveRel(float x, float y);
  void setVisible(bool visible);
  void setAnimation(char *animation);
  void pauseAnimation(bool on);
  void setBoundingBox(int width, int height, int offset_x, int offset_y);
  int getX();
  int getY();
  SDL_Rect *getBoundingBox();
  bool collides(Object *other);
  bool destroy();

  /* Update position and actions in the level. */
  virtual void update();

 protected:
  remar2d *gfx;
  SoundManager *sfx;
  int sprite_instance;
  float pos_x, pos_y;
  SDL_Rect bounding_box;
  bool animationPaused;
  char *name;

  /* Countdown to destruction (GRRRRRR) */
  int destroyTimer;

  /* Destroy this object */
  bool destroyMe;
};

#endif
