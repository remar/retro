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
  void nextLevel();
  bool blocksTakeTwoHits();
  void resetKills();
  void killed(EnemyType type);

 private:
  int skillLevel;
  int level; /* Level we're on */
  int lives;
  int score;
  int kills[7];
};

#endif
