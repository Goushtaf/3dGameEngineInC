#pragma once
#include "math3d.h"
#include <stddef.h>

typedef struct {
    Triangle3d *triangles;
    size_t count;
} ObjModel;
int compare_wrapper(const void *a, const void *b, void *arg);

ObjModel loadObjFile(const char *filename);

void sortTriangleArray(TriangleRenderData *triangleBuffer, int trueSize, const Vector3d *cam);



