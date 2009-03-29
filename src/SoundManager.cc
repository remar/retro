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
      "dot.wav",   /* 12 */
      "pause.wav", /* 13 */
      "start.wav", /* 14 */
      "scoretick.wav", /* 15 */
      "bonus.wav", /* 16 */
      "pickup.wav", /* 17 */
      "quake.wav", /* 18 */
      0
    };
  
  char *musicFiles[] =
    {
      "test.wav", /* 0 */
      "clear.wav", /* 1 */
      "bstage.wav", /* 2 */
      0
    };

  int files = 0;
  for(;soundFiles[files];files++);

  sounds = new Mix_Chunk*[files];

  printf("Loading sounds");
  for(int i = 0;soundFiles[i];i++)
    {
      char buf[1024];
      sprintf(buf, "../sfx/%s", soundFiles[i]);
      //printf("Loading sound \"%s\"\n", buf);
      printf(".");
      sounds[i] = Mix_LoadWAV(buf);
    }
  printf("\n");

  files = 0;
  for(;musicFiles[files];files++);

  songs = new Mix_Music*[files];

  printf("Loading music");
  for(int i = 0;musicFiles[i];i++)
    {
      char buf[1024];
      sprintf(buf, "../sfx/%s", musicFiles[i]);
      //printf("Loading music \"%s\"\n", buf);
      printf(".");
      songs[i] = Mix_LoadMUS(buf);
    }
  printf("\n");
}

SoundManager::~SoundManager()
{
  Mix_HaltChannel(-1);
  Mix_HaltMusic();
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
SoundManager::playMusic(int i, bool loop)
{
  Mix_PlayMusic(songs[i], loop ? -1 : 0);
}

void
SoundManager::stopMusic()
{
  Mix_HaltMusic();
}
