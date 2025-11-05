#include "math3d.h"
#include <math.h>
#include <stdlib.h>

void addScalarToThis(Vector3d *v, float dx, float dy, float dz){
    v -> x += dx;
    v -> y += dy;
    v -> z += dz;
}
void addVectorToThis(Vector3d *self, Vector3d *that){
    self -> x += (that -> x);
    self -> y += (that -> y);
    self-> z += (that -> z);
}
void subtractVectorToThis(Vector3d *self, Vector3d *that){
    self -> x -= (that -> x);
    self -> y -= (that -> y);
    self -> z -= (that -> z);
}
Vector3d addVector(const Vector3d *v1,const Vector3d *v2){
    Vector3d v = {v1 -> x + v2 -> x, v1 -> y + v2 -> y, v1 -> z + v2 -> z};
    return v;
}
Vector3d subVector(const Vector3d *v1, const Vector3d *v2){
    Vector3d v = {v1 -> x - v2 -> x, v1 -> y - v2 -> y, v1 -> z - v2 -> z};
    return v;
}
float magnitude(Vector3d v){
    return sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
}
Vector3d normalize(Vector3d v) {
    float mag = magnitude(v);
    if (mag == 0.0f) return (Vector3d){0, 0, 0};
    return (Vector3d){v.x / mag, v.y / mag, v.z / mag};
}

Vector3d getNorm(const Triangle3d *tri){
    Vector3d a = subVector(&tri -> vertices[1], &tri -> vertices[0]);
    Vector3d b = subVector(&tri -> vertices[2], &tri -> vertices[0]); 
    Vector3d norm = {
        (a.y*b.z)-(a.z*b.y),
        (a.z*b.x)-(a.x*b.z),
        (a.x*b.y)-(a.y*b.x)
    };
    return norm;
}
float dotProduct(Vector3d v1, Vector3d v2){
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}
float perpendicularity(Vector3d v1, Vector3d v2){
    float dot = dotProduct(v1, v2);
    float mag = magnitude(v1) * magnitude(v2);

    if (mag == 0.0f) return 0.0f;
    float ratio = fabsf(dot / mag);
    return 1.0f - ratio;
}
Cube3d initCube(void) {
    Cube3d cube = {
        .faces = {
            // south
            {{{0,0,0},{0,1,0},{1,1,0}}},
            {{{0,0,0},{1,1,0},{1,0,0}}},
            // east
            {{{1,0,0},{1,1,0},{1,1,1}}},
            {{{1,0,0},{1,1,1},{1,0,1}}},
            // north
            {{{1,0,1},{1,1,1},{0,1,1}}},
            {{{1,0,1},{0,1,1},{0,0,1}}},
            // west
            {{{0,0,1},{0,1,1},{0,1,0}}},
            {{{0,0,1},{0,1,0},{0,0,0}}},
            // top
            {{{0,1,0},{0,1,1},{1,1,1}}},
            {{{0,1,0},{1,1,1},{1,1,0}}},
            // bottom
            {{{1,0,1},{0,0,1},{0,0,0}}},
            {{{1,0,1},{0,0,0},{1,0,0}}}
        }
    };
    return cube;
}

// Multiplication matrice/vecteur
void MultiplyMatrixVector(const Vector3d *i, Vector3d *o, const Mat4x4 *m) {
    o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
    o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
    o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];

    float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];
    if (w != 0.0f) {
        o->x /= w;
        o->y /= w;
        o->z /= w;
    }
}

// Convertit coordonnées normalisées en pixels
Vector2d FromCenteredNormalized(float x, float y, int width, int height){
    Vector2d result;
    result.x = x * (width / 2.0f) + (width / 2.0f);
    result.y = y * (height / 2.0f) + (height / 2.0f);
    return result;
}


void MoveTriangle(Triangle3d *tri, float OffsetX, float OffsetY, float OffsetZ){
    addScalarToThis(&tri-> vertices[0], OffsetX, OffsetY, OffsetZ);
    addScalarToThis(&tri-> vertices[1], OffsetX, OffsetY, OffsetZ);
    addScalarToThis(&tri-> vertices[2], OffsetX, OffsetY, OffsetZ);
}
Vector3d midle(const Triangle3d *tri){
    Vector3d v = {(tri -> vertices[0].x + tri -> vertices[1].x + tri -> vertices[2].x)/3,
        (tri -> vertices[0].y + tri -> vertices[1].y + tri -> vertices[2].y)/3,
        (tri ->vertices[0].z + tri -> vertices[1].z + tri -> vertices[2].z)/3};
    return v;
}
float distanceBetween(const Vector3d *v1, const Vector3d *v2){
    Vector3d a = subVector(v1, v2);
    return sqrtf(a.x*a.x+a.y*a.y+a.z*a.z);
}

int compare(const Triangle3d *t1, const Triangle3d *t2, const Vector3d *cam) {
    Vector3d m1 = midle(t1);
    Vector3d m2 = midle(t2);

    float d1 = distanceBetween(&m1, cam);
    float d2 = distanceBetween(&m2, cam);

    return  (d1 > d2) - (d1 < d2);
}

