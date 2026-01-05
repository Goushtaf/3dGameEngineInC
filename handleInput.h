#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <stdbool.h>

typedef struct {
    bool quit;

    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    bool moveUp;
    bool moveDown;

    int mouseX;      // position absolue
    int mouseY;
    int mouseDeltaX; // mouvement relatif depuis la dernière frame
    int mouseDeltaY;

    bool mouseLeft;  // clic gauche
    bool mouseRight; // clic droit
} InputState;

void handleInput(InputState *input);

#endif
