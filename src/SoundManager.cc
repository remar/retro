#include "SoundManager.h"

SoundManager::SoundManager()
{
  char *soundFiles[] =
    {
      "bomb.wav",  /* 0 */
      "coin.wav",  /* 1 */
      "death.wav", /* 2 */
      "explo.wav", /* 3 */
      "jump.wav",  /* 4 */
      "pain.wav",  /* 5 */
      "shoot.wav", /* 6 */
      "spawn.wav", /* 7 */
      "splat.wav", /* 8 */
      "stun.wav",  /* 9 */
      "wake.wav",  /* 10 */
      "walk.wav",  /* 11 */
      0
    };
  
  char *musicFiles[] =
    {
      "test.wav",
      0
    };

  int files = 0;
  for(;soundFiles[files];files++);

  sounds = new Mix_Chunk*[files];

  for(int i = 0;soundFiles[i];i++)
    {
      char buf[1024];
      sprintf(buf, "../sfx/%s", soundFiles[i]);
      printf("Load sound %s\n", buf);
      sounds[i] = Mix_LoadWAV(buf);
    }

  files = 0;
  for(;musicFiles[files];files++);

  songs = new Mix_Music*[files];

  for(int i = 0;musicFiles[i];i++)
    {
      char buf[1024];
      sprintf(buf, "../sfx/%s", musicFiles[i]);
      songs[i] = Mix_LoadMUS(buf);
    }
}

int
SoundManager::playSound(int i, bool loop)
{
  return Mix_PlayChannel(-1, sounds[i], loop ? -1 : 0);
}

void
SoundManager::stopSound(int channel)
{
  Mix_HaltChannel(channel);
}

void
SoundManager::playMusic(int i)
{
  Mix_PlayMusic(songs[i], -1);
}
