#pragma once
#include <stddef.h>
typedef struct {
    float x, y;
} Vector2d;

typedef struct {
    float x, y, z;
} Vector3d;

typedef struct {
    Vector3d vertices[3];
} Triangle3d;

typedef struct {
    Triangle3d faces[12];
} Cube3d;

struct ObjModel;

typedef struct {
    float m[4][4];
} Mat4x4;

typedef struct {
    float a,b,c,d;
} Plane3d;

void addScalarToThis(Vector3d *v, float dx, float dy, float dz);

void addVectorToThis(Vector3d *self, Vector3d *that);

void subtractVectorToThis(Vector3d *self, Vector3d *that);

Vector3d addVector(const Vector3d *v1, const Vector3d *v2);

Vector3d subVector(const Vector3d *v1, const Vector3d *v2);

Vector3d mulVector(const Vector3d *v1, float scalar);

float magnitude(Vector3d v);

Vector3d normalize(Vector3d v);

Vector3d getNorm(const Triangle3d *tri);

Vector3d vectorCrossProduct(const Vector3d *v1, const Vector3d *v2);

float dotProduct(Vector3d v1, Vector3d v2);

float perpendicularity(Vector3d v1, Vector3d v2);

void MultiplyMatrixVector(const Vector3d *i, Vector3d *o, const Mat4x4 *m);

Vector2d FromCenteredNormalized(float x, float y, int width, int height);

Cube3d initCube(void);

void MoveTriangle(Triangle3d *tri, float OffsetX, float OffsetY, float OffsetZ);

Vector3d midle(const Triangle3d *tri);

float distanceBetween(const Vector3d *v1, const Vector3d *v2);

int compare(const Triangle3d *t1, const Triangle3d *t2, const Vector3d *cam);


Mat4x4 multiplyMatrixMatrix(const Mat4x4 *m1, const Mat4x4 *m2);

Mat4x4 createRotationMatrix(int axis, double fTheta);
 


















