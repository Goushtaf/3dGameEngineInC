#include <SDL2/SDL_scancode.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "handleInput.h"

void handleInput(InputState *input) {
    SDL_Event event;

    // Remet les deltas à zéro avant de lire les événements
    input->mouseDeltaX = 0;
    input->mouseDeltaY = 0;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            input->quit = true;

        // 🖱️ Mouvements de souris
        else if (event.type == SDL_MOUSEMOTION) {
            input->mouseX = -event.motion.x;
            input->mouseY = event.motion.y;
            input->mouseDeltaX += event.motion.xrel;
            input->mouseDeltaY += event.motion.yrel;
        }

        // 🖱️ Clics souris
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT)
                input->mouseLeft = true;
            if (event.button.button == SDL_BUTTON_RIGHT)
                input->mouseRight = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT)
                input->mouseLeft = false;
            if (event.button.button == SDL_BUTTON_RIGHT)
                input->mouseRight = false;
        }
    }

    // ⌨️ Clavier
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    input->moveForward  = keystate[SDL_SCANCODE_W];
    input->moveBackward = keystate[SDL_SCANCODE_S];
    input->moveLeft     = keystate[SDL_SCANCODE_A];
    input->moveRight    = keystate[SDL_SCANCODE_D];
    input->moveUp       = keystate[SDL_SCANCODE_SPACE];
    input->moveDown     = keystate[SDL_SCANCODE_LSHIFT];
}

