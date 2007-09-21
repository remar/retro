# PLEASE NOTE: 'remar2d' must be checked out as a neighbour to 'retro',
#              i.e. /home/benny/retro and /home/benny/remar2d

CFLAGS =	`sdl-config --cflags` -I../../remar2d
CPPFLAGS =	$(CFLAGS)
LIBS =		`sdl-config --libs`

PROGS =		retro

all:		$(PROGS)

RETRO =		retro.o Input.o GameLogic.o Hero.o Fuzz.o Object.o Level.o \
		Coin.o \
		../../remar2d/remar2d.o ../../remar2d/tinyxml.o \
		../../remar2d/tinystr.o ../../remar2d/tinyxmlerror.o \
		../../remar2d/tinyxmlparser.o ../../remar2d/Sprite.o \
		../../remar2d/Animation.o ../../remar2d/SpriteInstance.o \
		../../remar2d/TileSet.o ../../remar2d/Tile.o

retro:		$(RETRO)
		g++ -o $@ $^ $(LIBS)

clean:
		rm -f *.o *~ $(PROGS)
