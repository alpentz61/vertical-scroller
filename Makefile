CC=gcc
CFLAGS= -I. 
CXXFLAGS = -g -std=c++11 -Wall -pedantic 
LIBS=-lallegro -lallegro_image
DEPS = game.h object.h map.h
OBJ = main.o object.o map.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
