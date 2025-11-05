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
void DrawTriangle3d(SDL_Renderer *renderer, const TriangleRenderData *tri, int width, int height) {
    Vector2d a = FromCenteredNormalized(tri->tri.vertices[0].x, tri->tri.vertices[0].y, width, height);
    Vector2d b = FromCenteredNormalized(tri->tri.vertices[1].x, tri->tri.vertices[1].y, width, height);
    Vector2d c = FromCenteredNormalized(tri->tri.vertices[2].x, tri->tri.vertices[2].y, width, height);
    filledTrigonRGBA(renderer, a.x, a.y, b.x, b.y, c.x, c.y, UnNormalizeAlpha(tri->lightintensity), UnNormalizeAlpha(tri->lightintensity), UnNormalizeAlpha(tri->lightintensity), 255);
    //DrawLine2d(renderer, &a, &b);
    //DrawLine2d(renderer, &b, &c);
    //DrawLine2d(renderer, &a, &c);
}







