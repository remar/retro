#ifndef HERO_H
#define HERO_H

#include <remar2d.h>
#include <list>
#include "Object.h"
#include "Bullet.h"
#include "HUD.h"

class Hero : public Object
{
 public:
  Hero(remar2d *gfx, SoundManager *sfx, list<Bullet *> *bullets, HUD *hud);
  ~Hero();
  void moveRel(float xDir, float yDir); //, bool onGround);
  void setVisible(bool visible);
  void stop();
  void jump(bool on);
  bool jumps(int decrease);
  void shoot(); //int *bullets, Bullet **bullet);
  void die();
  void update();
  bool isBlinking() { return blinking; }
  bool isDead() { return dead; }
  void showNote(int ammo);
  void powerShot(); // Fire a POWER bullet on next shoot()
  void laserShot(); // Fire a LASER beam on next shoot()

 private:
  void updateAnimation(float xDir, float yDir);
  void setDirection(float dir);

  enum Direction
    {
      LEFT,
      RIGHT
    };

  enum Weapon
    {
      STANDARD,
      POWER,
      LASER
    };

  Direction direction;
  Weapon weapon;

  int jumpCounter;
  bool jumping;
  bool allowJumping;
  bool falling;

  bool blinking;
  bool dead;

  /* The flame sprite (displayed when jumping) */
  int flame;
  bool flameShown;
  Direction flameDirection;

  /* The "note" sprite (showing how many bullets are currently available) */
  int note;
  bool noteShown;
  int noteTimer; // Countdown how long note should be shown

  float oldXDir;
  float oldYDir;

  enum Animation
    {
      FLYING,
      WALKING,
      STANDING
    };

  Animation currentAnimation;
  
  /* channel that the walk sound is played on */
  int channel;

  int deathTimer;

  list<Bullet *> *bullets;

  HUD *hud;

  bool hasStoppedSound;
};

#endif
