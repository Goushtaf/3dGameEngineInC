
#include "loadObjFile.h"
#include "math3d.h"
#include <stdio.h>
#include <stdlib.h>

ObjModel loadObjFile(const char *filename) {
    ObjModel model = {NULL, 0};
    Vector3d *vertices = NULL;
    size_t vertexCount = 0, vertexCapacity = 0;
    Triangle3d *tris = NULL;
    size_t triCount = 0, triCapacity = 0;

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur : impossible d’ouvrir %s\n", filename);
        return model;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vector3d v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            if (vertexCount >= vertexCapacity) {
                vertexCapacity = vertexCapacity ? vertexCapacity * 2 : 64;
                vertices = realloc(vertices, vertexCapacity * sizeof(Vector3d));
            }
            vertices[vertexCount++] = v;
        } else if (line[0] == 'f' && line[1] == ' ') {
            int i1, i2, i3;
            sscanf(line, "f %d %d %d", &i1, &i2, &i3);
            if (triCount >= triCapacity) {
                triCapacity = triCapacity ? triCapacity * 2 : 64;
                tris = realloc(tris, triCapacity * sizeof(Triangle3d));
            }
            tris[triCount].vertices[0] = vertices[i1 - 1];
            tris[triCount].vertices[1] = vertices[i2 - 1];
            tris[triCount].vertices[2] = vertices[i3 - 1];
            triCount++;
        }
    }

    fclose(file);
    free(vertices);

    model.triangles = tris;
    model.count = triCount;
    return model;
}
