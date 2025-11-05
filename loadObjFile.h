#ifndef LOADOBJFILE_H
#define LOADOBJFILE_H

#include "math3d.h"
#include <stddef.h>

typedef struct {
    Triangle3d *triangles;
    size_t count;
} ObjModel;

ObjModel loadObjFile(const char *filename);

#endif


