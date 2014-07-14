#include "Headers/CullingEngine.h"



CullingEngine::CullingEngine(float nDist, float fDist, float ratio, float fov, Vec3<double> pos, Vec3<double> dir, Vec3<double> up){
    nearPlane = new Plane<double>();
    farPlane = new Plane<double>();
    rightPlane = new Plane<double>();
    leftPlane = new Plane<double>();
    topPlane = new Plane<double>();
    bottomPlane = new Plane<double>();

    updateCameraParameters(nDist,fDist,ratio,fov);
    updateOrigin(pos, dir, up);
    updatePlanes();
}

CullingEngine::~CullingEngine(){
    delete nearPlane;
    delete farPlane;
    delete rightPlane;
    delete leftPlane;
    delete bottomPlane;
    delete topPlane;
}


void CullingEngine::updateOrigin(Vec3<double> pos, Vec3<double> dir, Vec3<double> up){
    this->pos = Vec3<double>(pos);
    this->dir = Vec3<double>(dir);
    this-> up = Vec3<double>(up);

    updatePlanes();
}

void CullingEngine::updateCameraParameters(float nDist, float fDist, float ratio, float fov){
    this->nearDistance = nDist;
    this->farDistance = fDist;
    this->ratio = ratio;
    this->fov = fov;

    this->hNear = 2 * tan(fov/2) * nearDistance;
    this->wNear = hNear*ratio;

    this->hFar = 2 * tan(fov/2) * farDistance;
    this->wFar = hFar*ratio;

    updatePlanes();
}

void CullingEngine::updatePlanes(){
    Vec3<double> nearOrigin = pos + dir * nearDistance;
    Vec3<double> nearNormal = dir;
    nearPlane->update(nearOrigin,nearNormal);

    Vec3<double> farOrigin = pos + dir * farDistance;
    Vec3<double> farNormal = dir * -1;
    farPlane->update(farOrigin,farNormal);


    Vec3<double> right = dir.crossProduct(up);

    Vec3<double> rightRay = (nearOrigin + right* wNear / 2) - pos;
    rightRay.normalize();
    Vec3<double> rightNormal = up.crossProduct(rightRay);
    rightPlane->update(rightRay,rightNormal);

    Vec3<double> leftRay = (nearOrigin - right* wNear / 2) - pos;
    leftRay.normalize();
    Vec3<double> leftNormal = leftRay.crossProduct(up);
    leftPlane->update(leftRay, leftNormal);

    Vec3<double> topRay = (nearOrigin + up * hNear / 2) - pos;
    topRay.normalize();
    Vec3<double> topNormal = topRay.crossProduct(right);
    topPlane->update(topRay, topNormal);

    Vec3<double> bottomRay = (nearOrigin - up* hNear / 2) - pos;
    bottomRay.normalize();
    Vec3<double> bottomNormal = right.crossProduct(bottomRay);
    bottomPlane->update(bottomRay, bottomNormal);
}

int CullingEngine::isScreenVisible(Screen* screen){
    Vec3<double>** corners = screen->getCorners();
    int totalCornerVisible = 0;
    for(int i = 0; i < 4; i++){
        int isVisible = 1;
        double distance = bottomPlane->distanceFromPoint(*corners[i]);
        if(distance < 0){
            isVisible = 0;
        }
        distance = topPlane->distanceFromPoint(*corners[i]);
        if(distance < 0){
            isVisible = 0;
        }
        distance = leftPlane->distanceFromPoint(*corners[i]);
        if(distance < 0){
            isVisible = 0;
        }
        distance = rightPlane->distanceFromPoint(*corners[i]);
        if(distance < 0){
            isVisible = 0;
        }
        distance = nearPlane->distanceFromPoint(*corners[i]);
        if(distance < 0){
            isVisible = 0;
        }
        distance = farPlane->distanceFromPoint(*corners[i]);
        if(distance < 0){
            isVisible = 0;
        }


        if(isVisible){
            totalCornerVisible++;
        }
    }

    if(totalCornerVisible==4){
        return 1;
    }
    else if(totalCornerVisible==0){
        return 0;
    }
    else{
        return 2;
    }
}

