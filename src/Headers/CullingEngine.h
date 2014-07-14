#ifndef CULLINGENGINE_H_INCLUDED
#define CULLINGENGINE_H_INCLUDED

#define INSIDE 1
#define OUTSIDE 0
#define PARTIAL 2

#include "Screen.h"
#include "Utils/Plane.h"
#include "Utils/Vec3.h"

class Screen;

class CullingEngine{
    public :
        CullingEngine(float,float,float, float, Vec3<double>, Vec3<double>, Vec3<double>);
        ~CullingEngine();


        int isScreenVisible(Screen*);

        void updateOrigin(Vec3<double>,Vec3<double>,Vec3<double>);
        void updateCameraParameters(float, float, float, float);
    private :
        Plane<double> *nearPlane, *farPlane, *rightPlane, *leftPlane, *bottomPlane, *topPlane;

        Vec3<double> pos,dir,up;

        float nearDistance;
        float farDistance;
        float ratio;
        float fov;

        float hNear,hFar,wNear,wFar;

        void updatePlanes();
};
#endif // CULLINGENGINE_H_INCLUDED
