#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include "math3d.h"
#include "graphics.h"
#include "loadObjFile.h"
#include "handleInput.h"

int main(void) {
    int width = 2000;
    int height = 1080;
    InputState inputState = {0};
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "3dGameEngine (SDL2)", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, 0
    );
    ObjModel model = loadObjFile("mapTest1.obj");
    printf("Chargé %zu triangles !\n", model.count);
    TriangleRenderData triangleBuffer[model.count];



    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)height / (float)width;
       
    float moveSpeed = 10.0f;
    Light vLight = {
        {0.5f, -1.0f, 1.0f},
        {1.0f, 20.0f, 1.0f}
    };
    Mat4x4 matProj = createProjectionMatrix(fAspectRatio, fFov, fFar, fNear); 
    Vector3d camera = {0.0f, 0.0f, 0.0f}; // Position de la caméra
    float fYaw = 0.0f;
    float fXaw = 0.0f;
    Vector3d vLookDir = {0.0f, 0.0f, 1.0f};  // Direction initiale : regarder vers +Z
    
    bool running = true;
    Uint64 start = SDL_GetPerformanceCounter();
    double freq = (double)SDL_GetPerformanceFrequency();
    float now = 0;
    float last = 0;
    float fElapsedTime;
    



    while (running) {
        last = now;
        now = SDL_GetPerformanceCounter();
        fElapsedTime = (double)((now - last) * 1000 / (double)SDL_GetPerformanceFrequency()) / 1000.0;

        // Calcul du temps écoulé
        Uint64 now = SDL_GetPerformanceCounter();
        handleInput(&inputState);
        Vector3d vForward = mulVector(&vLookDir, moveSpeed * fElapsedTime);
        Vector3d vUp = {0.0f, 1.0f, 0.0f};
        Vector3d vDown = {0.0f, -1.0f, 0.0f};

        if (inputState.quit){running = false;}
        if (inputState.moveForward){camera = addVector(&camera, &vForward);}
        if (inputState.moveBackward){camera = subVector(&camera, &vForward);}
        if (inputState.moveLeft || inputState.moveRight) {
            Vector3d vRight = normalize(vectorCrossProduct(&vUp, &vLookDir));
            Vector3d vStrafe = mulVector(&vRight, moveSpeed * fElapsedTime);
            if (inputState.moveLeft)  camera = addVector(&camera, &vStrafe);
            if (inputState.moveRight) camera = subVector(&camera, &vStrafe);
        }
        if (inputState.moveUp){
            camera = addVector(&vUp, &camera);
        }
        if (inputState.moveDown) {
            camera = addVector(&vDown, &camera);
        }
        if (inputState.mouseX){
            fXaw += 0.01f * inputState.mouseDeltaY;
            if ((fXaw > (M_PI/2 - 0.05f))||(fXaw < (-M_PI/2 + 0.05f)))
            {
                fXaw -= 0.01f * inputState.mouseDeltaY;
            }

            
        }
        if (inputState.mouseY){fYaw += 0.01f * inputState.mouseDeltaX;}
               
        int trueSize = 0;

        // Appliquer la rotation à la direction de vue
        Vector3d vTarget = {0.0f, 0.0f, 1.0f};  // Direction initiale
        Mat4x4 matCameraRotX = createRotationMatrix(-1, fXaw); 
        Mat4x4 matCameraRotY = createRotationMatrix(0, fYaw);  // Rotation Y (yaw)
        Mat4x4 matCameraRot = multiplyMatrixMatrix(&matCameraRotX, &matCameraRotY);
        MultiplyMatrixVector(&vTarget, &vLookDir, &matCameraRot);
        vLookDir = normalize(vLookDir);  // Normaliser après rotation
        
        // Point ciblé = position caméra + direction de vue
        vTarget = addVector(&camera, &vLookDir);



        Mat4x4 matCam = pointAtMatrix(&camera, &vTarget, &vDown);

        Mat4x4 matView = Matrix_QuickInverse(&matCam);


        // Matrice de rotation
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (size_t i = 0; i < model.count; i++) {
            Triangle3d tri = model.triangles[i];
            Triangle3d triProjected;
            Triangle3d triViewed;
            MoveTriangle(&tri, -0.5f, -0.5f , 8.0f);
            
                                    
            float lightIntensity = computeLightIntensity(&tri, &vLight);


            
            if (!isPointingTowardCam(&tri, &camera)){ continue;}


            
            MultiplyMatrixVector(&tri.vertices[0], &triViewed.vertices[0], &matView);
            MultiplyMatrixVector(&tri.vertices[1], &triViewed.vertices[1], &matView);
            MultiplyMatrixVector(&tri.vertices[2], &triViewed.vertices[2], &matView);

            Vector3d avPos = getTriangleAveragePos(&triViewed);

            TriangleRenderData triViewedWithRenderData;
            triViewedWithRenderData.depth = magnitude(subVector(&avPos, &vLight.pos));
            triViewedWithRenderData.lightintensity = lightIntensity;
            triViewedWithRenderData.tri = triViewed;

            int nClippedTriangles = 0;
            TriangleRenderData clipped[2];
            // clip against near plane in view-space (déjà utilisé)
            nClippedTriangles = Triangle_ClipAgainstPlane(&(Vector3d){.x = 0.0f,.y=0.0f,.z=0.5f},
             &(Vector3d){.x = 0.0f,.y = 0.0f, .z = 1.0f},
             &triViewedWithRenderData, &clipped[0], &clipped[1]);

            // Si aucun triangle après clipping, on skip
            if (nClippedTriangles == 0) continue;

            for (int ci = 0; ci < nClippedTriangles; ci++){

                MultiplyMatrixVector(&clipped[ci].tri.vertices[0], &triProjected.vertices[0], &matProj);
                MultiplyMatrixVector(&clipped[ci].tri.vertices[1], &triProjected.vertices[1], &matProj);
                MultiplyMatrixVector(&clipped[ci].tri.vertices[2], &triProjected.vertices[2], &matProj);

                bool outsideXNeg = (triProjected.vertices[0].x < -1.0f) && (triProjected.vertices[1].x < -1.0f) && (triProjected.vertices[2].x < -1.0f);
                bool outsideXPos = (triProjected.vertices[0].x >  1.0f) && (triProjected.vertices[1].x >  1.0f) && (triProjected.vertices[2].x >  1.0f);
                bool outsideYNeg = (triProjected.vertices[0].y < -1.0f) && (triProjected.vertices[1].y < -1.0f) && (triProjected.vertices[2].y < -1.0f);
                bool outsideYPos = (triProjected.vertices[0].y >  1.0f) && (triProjected.vertices[1].y >  1.0f) && (triProjected.vertices[2].y >  1.0f);
                bool behindCam    = (triProjected.vertices[0].z <= 0.0f) && (triProjected.vertices[1].z <= 0.0f) && (triProjected.vertices[2].z <= 0.0f);

                if (outsideXNeg || outsideXPos || outsideYNeg || outsideYPos || behindCam) {
                    continue;
                }

                TriangleRenderData triProjectedRenderData;
                triProjectedRenderData.tri = triProjected;
                triProjectedRenderData.lightintensity = clipped[ci].lightintensity;
                triProjectedRenderData.depth = clipped[ci].depth;
                triangleBuffer[trueSize] = triProjectedRenderData;
                trueSize++;
            }
        }
        sortTriangleArray(triangleBuffer, trueSize);
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


