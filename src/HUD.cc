#include "HUD.h"

HUD::HUD(remar2d *gfx, ScoreKeeper *scoreKeeper)
{
  /*   Counter *score; */
  /*   Counter *topScore; */
  /*   Counter *time; */
  /*   Counter *lives; */
  /*   Counter *level; */
  /*   Counter *skill; */

  this->scoreKeeper = scoreKeeper;

  hudSprite = gfx->createSpriteInstance("hud");
  gfx->setAnimation(hudSprite, "normal");
  gfx->showSprite(hudSprite, true);
  gfx->moveSpriteAbs(hudSprite, 32, 8);
  gfx->neverRedraw(hudSprite, true);

  int lengths[6] = { 8, 8, 3, 2, 2, 1 };
  int pos_x[6] = {298, 522, 713, 298, 458, 618};
  int pos_y[6] = {20, 20, 20, 50, 50, 50};

  for(int i = 0;i < 6;i++)
    {
      counters[i] = new Counter(gfx, lengths[i]);
      counters[i]->setPosition(pos_x[i], pos_y[i]);
    }

  //enum Display { SCORE, TOP, TIME, LIVES, STAGE, SKILL, BULLET, RELOAD };

  counters[SCORE]->setCounter(scoreKeeper->getScore());
  counters[TOP]->setCounter(scoreKeeper->getTopScore());
  counters[TIME]->setCounter(200);
  counters[LIVES]->setCounter(scoreKeeper->getLives());
  counters[STAGE]->setCounter(scoreKeeper->getLevel());
  counters[SKILL]->setCounter(scoreKeeper->getSkillLevel());
}

void
HUD::update()
{

}

void
HUD::setValue(Display display, int value)
{
  counters[display]->setCounter(value);
}
