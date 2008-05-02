#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SDL_mixer.h"

class SoundManager
{
 public:
  SoundManager();
  ~SoundManager();
  int playSound(int i, bool loop = false);
  void stopSound(int channel);
  void playMusic(int i, bool loop = true);

 private:
  Mix_Chunk **sounds;
  Mix_Music **songs;
};

#endif
