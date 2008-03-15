# PLEASE NOTE: 'remar2d' must be checked out as a neighbour to 'retro',
#              i.e. /home/benny/retro and /home/benny/remar2d

CFLAGS =	`sdl-config --cflags` -I../../remar2d/src -g -Wall
CPPFLAGS =	$(CFLAGS)
LIBS =		`sdl-config --libs`

PROGS =		retro

all:		$(PROGS)

RETRO =		retro.o Input.o GameLogic.o Hero.o Fuzz.o Object.o Level.o \
		Coin.o Nest.o ScoreKeeper.o Spawner.o Bullet.o \
		../../remar2d/src/remar2d.o ../../remar2d/src/tinyxml.o \
		../../remar2d/src/tinystr.o ../../remar2d/src/tinyxmlerror.o \
		../../remar2d/src/tinyxmlparser.o ../../remar2d/src/Sprite.o \
		../../remar2d/src/Animation.o \
		../../remar2d/src/SpriteInstance.o \
		../../remar2d/src/TileSet.o ../../remar2d/src/Tile.o

retro:		$(RETRO)
		g++ -o $@ $^ $(LIBS)

clean:
		rm -f *.o *~ $(PROGS)
