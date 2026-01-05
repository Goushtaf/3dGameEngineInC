#include "graphics.h"
#include "math3d.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>




void DrawLine2d(SDL_Renderer *renderer, const Vector2d *start, const Vector2d *end) {
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
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

    
    DrawLine2d(renderer, &a, &b);
    DrawLine2d(renderer, &b, &c);
    DrawLine2d(renderer, &a, &c);
}

float computeLightIntensity(const Triangle3d *tri, const Vector3d *vLight){
    Vector3d normal = normalize(getNorm(tri));
    float lightIntensity = dotProduct(normal, *vLight);
    lightIntensity = fmaxf(0.0f, lightIntensity);       // clamp bas
    lightIntensity = fabsf(1.0f - lightIntensity);      // inversion
    lightIntensity = fminf(fmaxf(lightIntensity, 0.2f), 0.8f);
    return lightIntensity;
}
bool isPointingTowardCam(const Triangle3d *tri, const Vector3d *cam){
    Vector3d toCamera = normalize(subVector(cam, &tri -> vertices[0]));
    Vector3d normal = normalize(getNorm(tri));
    return dotProduct(normal, toCamera) > 0.0f;
}
Mat4x4 createProjectionMatrix(float fAspectRatio, float fFovDeg, float fFar, float fNear){
    float fFovRad = 1.0f / tanf(fFovDeg * 0.5f * (M_PI / 180.0f));
    Mat4x4 matProj = {0};
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;
    return matProj;
}

Mat4x4 pointAtMatrix(const Vector3d *pos, const Vector3d *target, Vector3d *up){
    
    Vector3d newForward = subVector(target, pos);
    newForward = normalize(newForward);

    Vector3d a = mulVector(&newForward, dotProduct(*up, newForward));
    Vector3d newUp = subVector(up, &a);
    newUp = normalize(newUp);

    Vector3d newRight = vectorCrossProduct(&newUp, &newForward);

    Mat4x4 matrix;
		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos->x;			matrix.m[3][1] = pos->y;			matrix.m[3][2] = pos->z;			matrix.m[3][3] = 1.0f;
		return matrix;

}


Mat4x4 Matrix_QuickInverse(const Mat4x4 *m) // Only for Rotation/Translation Matrices
	{
		Mat4x4 matrix;
		matrix.m[0][0] = m->m[0][0]; matrix.m[0][1] = m->m[1][0]; matrix.m[0][2] = m->m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m->m[0][1]; matrix.m[1][1] = m->m[1][1]; matrix.m[1][2] = m->m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m->m[0][2]; matrix.m[2][1] = m->m[1][2]; matrix.m[2][2] = m->m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m->m[3][0] * matrix.m[0][0] + m->m[3][1] * matrix.m[1][0] + m->m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m->m[3][0] * matrix.m[0][1] + m->m[3][1] * matrix.m[1][1] + m->m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m->m[3][0] * matrix.m[0][2] + m->m[3][1] * matrix.m[1][2] + m->m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
Vector3d Vector_IntersectsPlane(Vector3d *planeP, Vector3d *planeN, Vector3d *lineStart, Vector3d *lineEnd){
    float ad = dotProduct(*lineStart, *planeN);
    float bd = dotProduct(*lineEnd, *planeN);
    float plane_d = dotProduct(*planeP, *planeN);
    float t = (plane_d - ad) / (bd - ad);
    Vector3d lineStartToEnd = subVector(lineEnd, lineStart);
    Vector3d lineToIntersect = mulVector(&lineStartToEnd, t); 
    return addVector(lineStart, &lineToIntersect);
}
int Triangle_ClipAgainstPlane(Vector3d *planeP, Vector3d *planeN, TriangleRenderData *inTri, TriangleRenderData *outTri1, TriangleRenderData *outTri2)
{
    Vector3d normalizedN = normalize(*planeN);
    Vector3d insidePoints[3]; int nInsidePointCount = 0;
    Vector3d outsidePoints[3]; int nOutsidePointCount = 0;

    float d0 = distFromPlaneToVector(planeP, &normalizedN, &inTri->tri.vertices[0]);
    float d1 = distFromPlaneToVector(planeP, &normalizedN, &inTri->tri.vertices[1]);
    float d2 = distFromPlaneToVector(planeP, &normalizedN, &inTri->tri.vertices[2]);

    if (d0>=0){ insidePoints[nInsidePointCount++] = inTri->tri.vertices[0];}
    else{outsidePoints[nOutsidePointCount++] = inTri->tri.vertices[0];}
    if (d1>=0){ insidePoints[nInsidePointCount++] = inTri->tri.vertices[1];}
    else{outsidePoints[nOutsidePointCount++] = inTri->tri.vertices[1];}
    if (d2>=0){ insidePoints[nInsidePointCount++] = inTri->tri.vertices[2];}
    else{outsidePoints[nOutsidePointCount++] = inTri->tri.vertices[2];}

    if (nInsidePointCount==0){
        return 0;
    }
    if(nInsidePointCount==3){
        *outTri1 = *inTri;
        return 1;
    }
    if (nInsidePointCount == 1 && nOutsidePointCount == 2){
        outTri1->depth = inTri->depth;
        outTri1->lightintensity = inTri->lightintensity;

        outTri1->tri.vertices[0] = insidePoints[0];
        outTri1->tri.vertices[1] = Vector_IntersectsPlane(planeP, &normalizedN, &insidePoints[0], &outsidePoints[0]);
        outTri1->tri.vertices[2] = Vector_IntersectsPlane(planeP, &normalizedN, &insidePoints[0], &outsidePoints[1]);

        return 1;
    }
    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
        outTri1->depth = inTri->depth;
        outTri1->lightintensity = inTri->lightintensity;

        outTri2->depth = inTri->depth;
        outTri2->lightintensity = inTri->lightintensity;

        outTri1->tri.vertices[0] = insidePoints[0];
        outTri1->tri.vertices[1] = insidePoints[1];
        outTri1->tri.vertices[2] = Vector_IntersectsPlane(planeP, &normalizedN, &insidePoints[0], &outsidePoints[0]);

        outTri2->tri.vertices[0] = insidePoints[1];
        outTri2->tri.vertices[1] = outTri1->tri.vertices[2];
        outTri2->tri.vertices[2] = Vector_IntersectsPlane(planeP, &normalizedN, &insidePoints[1], &outsidePoints[0]);

        return 2;
    }
    return 0;
}

float distFromPlaneToVector(Vector3d *planeP, Vector3d *planeN, Vector3d *point){
    *planeN = normalize(*planeN);
    Vector3d toPoint = subVector(point, planeP);
    return dotProduct(toPoint, *planeN);
}





