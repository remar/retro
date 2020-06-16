# PLEASE NOTE: 'cats-cpp' must be checked out as a neighbour to 'retro',
#              i.e. /home/benny/retro and /home/benny/cats-cpp

CFLAGS =	`sdl2-config --cflags` -g -Wall -DENABLE_BINRELOC -I../../cats-cpp/src
CPPFLAGS =	$(CFLAGS) -std=c++11

# Uncomment the first LIBS line and comment out the other LIBS line to
# compile without SDL_Mixer
#LIBS =		`sdl2-config --libs` ../../cats-cpp/src/libcats.a -lSDL2_image
LIBS =		`sdl2-config --libs` ../../cats-cpp/src/libcats.a -lSDL2_image -lSDL2_mixer

RETRO =		retro.o binreloc.o datadir.o Input.o GameLogic.o Menu.o Counter.o \
		ScoreKeeper.o FileManager.o GameMode.o SoundManager.o Level.o \
		Field.o Object.o BrokenBlock.o Explosion.o Hero.o
#		Fuzz.o \
#		Coin.o Nest.o Spawner.o Bullet.o \
#		Smoke.o Enemy.o Counter.o Drone.o \
#		Collectible.o HUD.o Menu.o ScoreScreen.o \
#		BulletHandler.o BonusLevel.o BonusScoreScreen.o \
#		SpaceViper.o SpaceViperBody.o BountyHunter.o \
#		HunterBullet.o DamagingExplosion.o Bonus.o BonusSpawner.o \
#		PowerBullet.o LaserBeam.o Helmet.o RedFuzz.o GoldDrone.o \
#		DireSpaceViper.o DireSpaceViperBody.o DarkBountyHunter.o \
#		DarkHunterBullet.o DarkExplosion.o ScoreSign.o

retrobattle:	$(RETRO) #../../remar2d/lib/libremar2d.a
		g++ -o $@ $^ $(LIBS)
		mv $@ ..

clean:
		rm -f *.o *~ core* ../retrobattle
