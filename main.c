
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>

#include "math3d.h"
#include "graphics.h"
#include "loadObjFile.h"
#include "handleInput.h"

int main(void) {
    int width = 800;
    int height = 450;
    InputState inputState = {0};
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "3dGameEngine (SDL2)", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, 0
    );
    ObjModel model = loadObjFile("utah_teapot.obj");
    printf("Chargé %zu triangles !\n", model.count);
    TriangleRenderData triangleBuffer[model.count];



    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)height / (float)width;
    float fFovRad = 1.0f / tanf(fFov * 0.5f * (M_PI / 180.0f));
    
    float dx = -0.5f;
    float dy = -0.5f;
    float dz = 5.0f;
    Vector3d vLight = {1.0f, 1.0f, 0.0f};
    Mat4x4 matProj = {0};
    Vector3d camera = {0.0f, 0.0f, 0.0f}; // Position de la caméra
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    bool running = true;
    Uint64 start = SDL_GetPerformanceCounter();
    double freq = (double)SDL_GetPerformanceFrequency();

    while (running) {
        
        handleInput(&inputState);

        if (inputState.quit){running = false;}
        
        int trueSize = 0;

        // Calcul du temps écoulé
        Uint64 now = SDL_GetPerformanceCounter();
        double time = (double)(now - start) / freq;
        double timeInRad = time;

        // Matrice de rotation
        Mat4x4 matRot = {0};
        matRot.m[0][0] = cos(timeInRad);
        matRot.m[0][1] = -sin(timeInRad);
        matRot.m[1][0] = cos(timeInRad) * sin(timeInRad);
        matRot.m[1][1] = cos(timeInRad) * cos(timeInRad);
        matRot.m[1][2] = -sin(timeInRad);
        matRot.m[2][0] = sin(timeInRad) * sin(timeInRad);
        matRot.m[2][1] = cos(timeInRad) * sin(timeInRad);
        matRot.m[2][2] = cos(timeInRad);
        matRot.m[3][3] = 1.0f;


        // Efface l’écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Couleur de dessin (noir)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (size_t i = 0; i < model.count; i++) {
            Triangle3d tri = model.triangles[i];
            Triangle3d triRotated;
            Triangle3d triProjected;

            MultiplyMatrixVector(&tri.vertices[0], &triRotated.vertices[0], &matRot);
            MultiplyMatrixVector(&tri.vertices[1], &triRotated.vertices[1], &matRot);
            MultiplyMatrixVector(&tri.vertices[2], &triRotated.vertices[2], &matRot);

            MoveTriangle(&triRotated, dx, dy , dz);
            
            // Calculer la normale et vérifier si la face est visible
            Vector3d normal = normalize(getNorm(&triRotated));
            
            Vector3d toCamera = subVector(&camera, &triRotated.vertices[0]);

            toCamera = normalize(toCamera);

            float dp = dotProduct(normal, toCamera);
                // Test de visibilité : si la normale pointe vers la caméra
            if (dp <= 0.0f) continue;
            
            
           
            float lightIntensity = dotProduct(normal, vLight);
            
            if (lightIntensity < 0.0f) lightIntensity = 0.0f;
            lightIntensity = fabsf(1 - lightIntensity);
            if (lightIntensity > 0.9)lightIntensity = 0.8f;
            if (lightIntensity < 0.15f)lightIntensity = 0.2f;
            
            
            
            MultiplyMatrixVector(&triRotated.vertices[0], &triProjected.vertices[0], &matProj);
            MultiplyMatrixVector(&triRotated.vertices[1], &triProjected.vertices[1], &matProj);
            MultiplyMatrixVector(&triRotated.vertices[2], &triProjected.vertices[2], &matProj);

            TriangleRenderData triProjectedRenderData;
            triProjectedRenderData.tri = triProjected;
            triProjectedRenderData.lightintensity = lightIntensity;
            triangleBuffer[trueSize] = triProjectedRenderData;
            trueSize++;
        }
        sortTriangleArray(triangleBuffer, trueSize, &camera);
        for (int i = 0; i < trueSize; ++i){
            DrawTriangle3d(renderer, &triangleBuffer[i], width, height);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ≈60 fps
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


