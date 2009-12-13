#ifndef DIRE_SPACE_VIPER_BODY_H
#define DIRE_SPACE_VIPER_BODY_H

#include "SpaceViperBody.h"

class DireSpaceViper; // So sorry :(

class DireSpaceViperBody : public SpaceViperBody
{
 public:
  DireSpaceViperBody(remar2d *gfx, SoundManager *sfx, bool bright,
		     SpaceViper *head);

};

#endif
