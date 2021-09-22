CC = gcc

INCLUDEFLAGS = -Ilib/glfw/include -Ilib/glew/include -Ilib/cglm/include -Isrc/modules
LDFLAGS = -lm -lGL -lX11 -lpthread -ldl
LIBS = lib/glew/lib/libGLEW.a lib/glfw/src/libglfw3.a lib/cglm/libcglm.a
BIN = bin/game.x86_64
MAIN = src/main.c

SRC = $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ = $(SRC:.c=.o)

.PHONY: all run

all: game

run: game
	printf "Starting program...\n\n\n\n\n\n\n"
	./$(BIN)

game: $(OBJ)
	$(CC) $(MAIN) $(LDFLAGS) $(INCLUDEFLAGS) $(LIBS) $^ -o $(BIN)
	$(MAKE) clean

%.o: %.c
	$(CC) $(INCLUDEFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)