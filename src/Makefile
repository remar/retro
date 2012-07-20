# PLEASE NOTE: 'remar2d' must be checked out as a neighbour to 'retro',
#              i.e. /home/benny/retro and /home/benny/remar2d

CFLAGS =	`sdl-config --cflags` -g -Wall -I../../remar2d/include -DENABLE_BINRELOC
CPPFLAGS =	$(CFLAGS)

# Uncomment the first LIBS line and comment out the other LIBS line to
# compile without SDL_Mixer
#LIBS =		`sdl-config --libs` ../../remar2d/lib/libremar2d.a
LIBS =		`sdl-config --libs` -lSDL_mixer ../../remar2d/lib/libremar2d.a

RETRO =		retro.o Input.o GameLogic.o Hero.o Fuzz.o Object.o Level.o \
		Coin.o Nest.o ScoreKeeper.o Spawner.o Bullet.o SoundManager.o \
		BrokenBlock.o Explosion.o Smoke.o Enemy.o Counter.o Drone.o \
		Field.o Collectible.o HUD.o Menu.o ScoreScreen.o GameMode.o \
		FileManager.o BulletHandler.o BonusLevel.o BonusScoreScreen.o \
		SpaceViper.o SpaceViperBody.o BountyHunter.o \
		HunterBullet.o DamagingExplosion.o Bonus.o BonusSpawner.o \
		PowerBullet.o LaserBeam.o Helmet.o RedFuzz.o GoldDrone.o \
		DireSpaceViper.o DireSpaceViperBody.o DarkBountyHunter.o \
		DarkHunterBullet.o DarkExplosion.o ScoreSign.o binreloc.o \
		datadir.o

retrobattle:	$(RETRO) ../../remar2d/lib/libremar2d.a
		g++ -o $@ $^ $(LIBS)
		mv $@ ..

clean:
		rm -f *.o *~ core* ../retrobattle
