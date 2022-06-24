IDIR=include
CPP=g++
LD=g++

OBJDIR=obj
SRCDIR=src
BINDIR=bin
LIBDIR=lib

CFLAGS=-I$(IDIR)
LDFLAGS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf
DBGFLAGS=-g -Wall

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) $(DBGFLAGS) $(CFLAGS) -c $^ -o $@

game: $(OBJ)
	$(LD) $(DBGFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $(BINDIR)/$@

.PHONY: run
run: game
	$(BINDIR)/game

.PHONY: clean
clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
