#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SDL_mixer.h"

class SoundManager
{
 public:
  SoundManager();
  int playSound(int i, bool loop);
  void stopSound(int channel);
  void playMusic(int i);

 private:
  Mix_Chunk **sounds;
  Mix_Music **songs;
};

#endif
