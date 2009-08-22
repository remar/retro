#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#define NO_SDL_MIXER

#ifndef NO_SDL_MIXER
#include "SDL_mixer.h"
#endif

class SoundManager
{
 public:
  SoundManager();
  ~SoundManager();
  int playSound(int i, bool loop = false);
  void stopSound(int channel);
  void playMusic(int i, bool loop = true);
  void stopMusic();

 private:
#ifndef NO_SDL_MIXER
  Mix_Chunk **sounds;
  Mix_Music **songs;
#endif

};

#endif
