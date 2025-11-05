# --- Variables ---
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 `pkg-config --cflags sdl2`
LDFLAGS = `pkg-config --libs sdl2` -lSDL2_gfx -lm
SRC = main.c math3d.c graphics.c loadObjFile.c handleInput.c
OBJ = $(SRC:.c=.o)
TARGET = main

# --- Règles principales ---
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile puis exécute
run: $(TARGET)
	./$(TARGET)

# Nettoyage
clean:
	rm -f $(OBJ) $(TARGET)
