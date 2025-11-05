#include <stdbool.h>
#include <SDL2/SDL.h>
#include "handleInput.h"

void handleInput(InputState *input){
    SDL_Event event;
     switch (event.type){
        case SDL_QUIT:
             input->quit = true; break;
        case SDL_KEYDOWN:
             switch (event.key.keysym.sym){
                case SDLK_SPACE:
                    input->stop = !(input->stop); break;
                case SDLK_w:
                    input->moveForward = true;
                case SDLK_s:
                    input->moveBackward = true;
                case SDLK_d:
                    input->moveRight = true;
                case SDLK_a:
                    input->moveLeft = true;
        } 
        case SDL_KEYUP:
            switch (event.key.keysym.sym){
                case SDLK_w:
                    input->moveForward = false;
                case SDLK_s:
                    input->moveBackward = false;
                case SDLK_d:
                    input->moveRight = false;
                case SDLK_a:
                    input->moveLeft = false;
        }
        case SDL_MOUSEMOTION:
            input->mouseX = event.motion.xrel;
            input->mouseY = event.motion.yrel;
            break;
     }
}
