IDIR =include
CPP=g++
CFLAGS=-I$(IDIR)

CDIR=src
BDIR=bin
LDIR =lib

LIBS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf

DEPS = $(wildcard $(IDIR)/*.h)

SRC = $(wildcard $(CDIR)/*.cpp)

game: $(SRC)
	$(CPP) -g -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

