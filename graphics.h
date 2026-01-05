#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "math3d.h"

typedef struct {
    Triangle3d tri;
    float lightintensity;
    float depth;
} TriangleRenderData;

void DrawTriangle3d(SDL_Renderer *renderer, const TriangleRenderData *tri, int width, int height);
float UnNormalizeAlpha(float alpha);
void DrawLine2d(SDL_Renderer *renderer, const Vector2d *start, const Vector2d *end);

float computeLightIntensity(const Triangle3d *tri, const Vector3d *vLight);
bool isPointingTowardCam(const Triangle3d *tri, const Vector3d *cam);
Mat4x4 createProjectionMatrix(float fAspectRatio, float fFovDeg, float fFar, float fNear);
Mat4x4 pointAtMatrix(const Vector3d *pos, const Vector3d *target, Vector3d *up);
Mat4x4 Matrix_QuickInverse(const Mat4x4 *m);

Vector3d Vector_IntersectsPlane(Vector3d *planeP, Vector3d *planeN, Vector3d *lineStart, Vector3d *lineEnd);
int Triangle_ClipAgainstPlane(Vector3d *planeP, Vector3d *planeN, TriangleRenderData *inTri, TriangleRenderData *outTri1, TriangleRenderData *outTri2);
float distFromPlaneToVector(Vector3d *planeP, Vector3d *planeN, Vector3d *point);

#endif
