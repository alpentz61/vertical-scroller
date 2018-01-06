CC=gcc
CFLAGS=-I.
LIBS=-lallegro -lallegro_image
DEPS = object.h
OBJ = main.o object.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
