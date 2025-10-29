#include "raylib.h"

// Définition du type Vector2d
typedef struct {
    float x, y;
} Vector2d;

// Fonctions
Vector2d FromCenteredNormalized(Vector2d normalized, int *pWidth, int *pHeight);
void DrawLine2d(Vector2d start, Vector2d end, int *pWidth, int *pHeight);

// Convertit un point normalisé (-1..1) en pixels écran
Vector2d FromCenteredNormalized(Vector2d normalized, int *pWidth, int *pHeight) {
    Vector2d result;
    result.x = normalized.x * (*pWidth / 2.0f) + (*pWidth / 2.0f);
    result.y = -normalized.y * (*pHeight / 2.0f) + (*pHeight / 2.0f);
    return result;
}

// Dessine une ligne normalisée
void DrawLine2d(Vector2d start, Vector2d end, int *pWidth, int *pHeight) {
    Vector2d unNormalizedStart = FromCenteredNormalized(start, pWidth, pHeight);
    Vector2d unNormalizedEnd = FromCenteredNormalized(end, pWidth, pHeight);
    DrawLine(unNormalizedStart.x, unNormalizedStart.y, unNormalizedEnd.x, unNormalizedEnd.y, BLACK);
}

int main(void)
{
    static int width = 800;
    static int height = 450;
    InitWindow(width, height, "3dGameEngine");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2d v1 = {-1, 0};  // point gauche
        Vector2d v2 = {1, 0};   // point droit
        DrawLine2d(v1, v2, &width, &height);  // on passe les pointeurs

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
