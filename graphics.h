#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "math3d.h"

void DrawTriangle3d(SDL_Renderer *renderer, const Triangle3d *tri, float alpha, int width, int height);
float UnNormalizeAlpha(float alpha);
void DrawLine2d(SDL_Renderer *renderer, const Vector2d *start, const Vector2d *end);

#endif
