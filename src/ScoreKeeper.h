#ifndef SCORE_KEEPER_H
#define SCORE_KEEPER_H

#include <stdio.h>

class ScoreKeeper
{
 public:
  ScoreKeeper();
  ~ScoreKeeper();

  enum EnemyType
    {
      Fuzz = 0,
      Drone,
      SpaceViper,
      BountyHunter,
      GoldDrone,
      DireSpaceViper,
      DarkBountyHunter
    };

  bool fastFuzzes();
  bool redFuzzes();

  int numberOfEnemy(EnemyType type);

  void setSkillLevel(int level);
  int  getSkillLevel();
  void incSkillLevel();

  int getLevel();
  void setLevel(int lev);
  void nextLevel();

  int getBonusLevel() { return bonusLevel; }
  void setBonusLevel(int lev) { bonusLevel = lev; }
  void nextBonusLevel() { (++bonusLevel > 8) ? bonusLevel = 1:0; }

  void levelCompleted() { ++levelsCompleted; }
  bool nextLevelIsBonus();    

  bool blocksTakeTwoHits();

  void resetKills();
  void killed(EnemyType type);
  int howManyKilled(EnemyType type);

  void heroKilled();
  int getLives();
  void setLives(int l);

  void addScore(int s);
  void setScore(int s);
  int getScore();

  void setTopScore(int s);
  int getTopScore();

  void calculateScore();

  void setTimer(int t) { timer = t; }
  int getTimer() { return timer; }
  void decTimer() { timer--; }

  void resetCoins() { coinsCollected = 0; coinsInLevel = 0; }
  void addCoin() { coinsInLevel++; }
  void collectCoin() { coinsCollected++; }
  int getCoinsCollected() { return coinsCollected; }
  int getCoinsInLevel() { return coinsInLevel; }
  int coinsLeft() { return coinsInLevel - coinsCollected; }

  void setNoBonus(bool bonus) { noBonus = bonus; }

  bool getNoBonus() { return noBonus; }

  void reset();

 private:
  int skillLevel;

  int level; /* Level we're on */
  int bonusLevel;

  int levelsCompleted;

  int lives;

  int score;
  int top;

  int kills[7];

  int timer;

  int coinsCollected;
  int coinsInLevel;

  bool noBonus;

  int nextExtraLife;
};

#endif
