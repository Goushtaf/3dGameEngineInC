#include "graphics.h"
#include "math3d.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>




void DrawLine2d(SDL_Renderer *renderer, const Vector2d *start, const Vector2d *end) {
      SDL_RenderDrawLine(renderer, (int)start ->x, (int)start ->y, (int)end->x, (int)end->y);
}

float UnNormalizeAlpha(float alpha){
    return (int)(alpha*255);
}



// Dessine un triangle projeté
void DrawTriangle3d(SDL_Renderer *renderer, const Triangle3d *tri, float alpha, int width, int height) {
    Vector2d a = FromCenteredNormalized(tri->vertices[0].x, tri->vertices[0].y, width, height);
    Vector2d b = FromCenteredNormalized(tri->vertices[1].x, tri->vertices[1].y, width, height);
    Vector2d c = FromCenteredNormalized(tri->vertices[2].x, tri->vertices[2].y, width, height);
    filledTrigonRGBA(renderer, a.x, a.y, b.x, b.y, c.x, c.y, 0, 0, 0, alpha);
    DrawLine2d(renderer, &a, &b);
    DrawLine2d(renderer, &b, &c);
    DrawLine2d(renderer, &a, &c);
}







