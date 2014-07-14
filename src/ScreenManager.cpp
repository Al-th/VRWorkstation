#include "Headers/ScreenManager.h"

#define SCREEN_WIDTH 32.0
#define SCREEN_HEIGHT 20.0
#define OPTIMAL_DISTANCE 65.0

ScreenManager::ScreenManager(int nbScreens){
    this->nbScreens = nbScreens;
    screenPool = new Screen*[nbScreens];
    for(int i = 0; i < nbScreens; i++){
        Vec3<double>** corners = computeCorners(i,nbScreens);
        screenPool[i] = new Screen(i,corners);
    }
}

ScreenManager::~ScreenManager(){
    for(int i = 0 ; i < nbScreens; i++){
        delete screenPool[i];
    }
    delete [] screenPool;
}

void ScreenManager::bindWindowToScreen(Window* window, int screenID){
    screenPool[screenID]->bindWindow(window);
}

Vec3<double>** ScreenManager::computeCorners(int screenID, int nbScreens){
    Vec3<double>** corners = new Vec3<double>*[4];
    float screenUnitAngle = 360.0 / nbScreens;

    Vec3<double> *unitVector = new Vec3<double>(0,0,1);

    for(int i = 0; i < 4; i++){
        corners[i] = new Vec3<double>(*unitVector);
    }


    double horizontalHalfAngle = atan(SCREEN_WIDTH/(2*OPTIMAL_DISTANCE));
    double verticalHalfAngle = atan(SCREEN_HEIGHT/(2*OPTIMAL_DISTANCE));

    float pi = atan(1)*4;
    horizontalHalfAngle = horizontalHalfAngle*180/pi;
    verticalHalfAngle = verticalHalfAngle*180/pi;

    corners[0]->rotatePitch(-horizontalHalfAngle);
    corners[0]->rotateRoll(verticalHalfAngle);

    corners[1]->rotatePitch(-horizontalHalfAngle);
    corners[1]->rotateRoll(-verticalHalfAngle);

    corners[2]->rotatePitch(horizontalHalfAngle);
    corners[2]->rotateRoll(verticalHalfAngle);

    corners[3]->rotatePitch(horizontalHalfAngle);
    corners[3]->rotateRoll(-verticalHalfAngle);


    for(int i = 0 ; i < 4; i++){
        *corners[i] *= OPTIMAL_DISTANCE;
        corners[i]->rotatePitch(screenID*screenUnitAngle);
    }

    return corners;
}


void ScreenManager::renderFunction(CullingEngine* cullingEngine){
    for(int i = 0 ; i < nbScreens; i++){
        screenPool[i]->renderFunction(cullingEngine);
    }
}

void ScreenManager::idleFunction(CullingEngine* cullingEngine){
    for(int i = 0 ; i < nbScreens; i++){
        screenPool[i]->idleFunction(cullingEngine);
    }

}
