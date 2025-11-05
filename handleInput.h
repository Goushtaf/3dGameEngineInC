#include <SDL2/SDL.h>
#include <stdbool.h>
#ifndef HANDLEINPUT
#define HANDLEINPUT

typedef struct{
    bool quit;
    bool moveForward;
    bool moveBackward;
    bool moveUp;
    bool moveDown;
    bool moveRight;
    bool moveLeft;
    int mouseX;
    int mouseY;
    bool stop;
} InputState;

void handleInput(InputState * input);
#endif
