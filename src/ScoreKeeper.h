#ifndef SCORE_KEEPER_H
#define SCORE_KEEPER_H

class ScoreKeeper
{
 public:
  ScoreKeeper();

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
/*   int numberOfBlueFuzzes(); */
/*   int numberOfRedFuzzes(); */

  void setSkillLevel(int level);
  int  getSkillLevel();

  int getLevel();
  void setLevel(int lev);
  void nextLevel();

  bool blocksTakeTwoHits();
  void resetKills();
  void killed(EnemyType type);
  void heroKilled();
  int getLives();

  void addScore(int s);
  void setScore(int s);
  int getScore();

  void setTopScore(int s);
  int getTopScore();

  void calculateScore();

  void setTimer(int t) { timer = t; }
  int getTimer() { return timer; }
  int decTimer() { timer--; }

 private:
  int skillLevel;
  int level; /* Level we're on */
  int lives;

  int score;
  int top;

  int kills[7];

  int timer;
};

#endif
