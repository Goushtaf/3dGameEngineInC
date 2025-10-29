
# === Makefile pour ton projet Raylib ===
CC = gcc
CFLAGS = -Wall -std=c99 -I/mingw64/include
LDFLAGS = -L/mingw64/lib -lraylib -lopengl32 -lgdi32 -lwinmm

TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
