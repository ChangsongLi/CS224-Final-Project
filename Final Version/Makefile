#
# A makefile for the Project
#
# May 9th
# Jason
#
CC=gcc
LDFLAGS=-lSDL -lSDL_image -lSDL_ttf -lSDL_gfx
# -lSDL_mixer
CFLAGS=-g -O0 -Wall

PROGS=game

all: $(PROGS)

game: game.c game.h linked_list.o support.o
	$(CC) game.c linked_list.o support.o -o game $(CFLAGS) $(LDFLAGS)

linked_list.o: linked_list.c linked_list.h
	$(CC) linked_list.c -c -o linked_list.o $(CFLAGS)

support.o: support.c support.h
	$(CC) support.c -c -o support.o $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(PROGS)
	$(RM) *.o
