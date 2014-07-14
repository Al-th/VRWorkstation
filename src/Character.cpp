#include "Headers/Character.h"


Character::Character(){
    pos = Vec3<double>(0,0,0);
    dir = Vec3<double>(0,0,-1);
    up = Vec3<double>(0,1,0);
    rotationalVelocity = -0.05;
    velocity = 0.5;
}

Character::~Character(){

}

Vec3<double> Character::getPos(){
    return pos;
}

Vec3<double> Character::getDir(){
    return dir;
}

Vec3<double> Character::getUp(){
    return up;
}

void Character::mouseFunction(int deltaX, int deltaY, float deltaTime){
    updateDir(deltaX, deltaY, deltaTime);
}

void Character::keyboardFunction(unsigned char key, float deltaTime){
    updatePos(key,deltaTime);
}

void Character::updatePos(unsigned char key, float deltaTime){
    Vec3<double> travelDistance = dir * (velocity*deltaTime);
    switch(key){
        case 122:
                pos += travelDistance ;
            break;
        case 115:
                pos -= travelDistance ;
            break;
        default :
            break;
    }
}

void Character::updateDir(int deltaX, int deltaY, float deltaTime){
    dir.rotatePitch(this->rotationalVelocity*deltaX*deltaTime);
}
