#include "Character.hpp"


Character::Character(){
    pos = new Point(0,0,30);
    dir = new Point(0,0,-1);
    up = new Point(0,1,0);
    rotationalVelocity = 0.05;
    translationalVelocity = 0.5;
}

Character::~Character(){

}

Point* Character::getPos(){
    return pos;
}

Point* Character::getDir(){
    return dir;
}


Point* Character::getUp(){
    return up;
}

void Character::updatePos(unsigned char key, float deltaTime){
    Point* p = new Point(*getDir());
    p->multiply(translationalVelocity*deltaTime);

    switch(key){
        case 122:
                pos->add(p);
            break;
        case 115:
                pos->substract(p);
                cout << pos->z << endl;
            break;
        default :
            break;
    }

    delete p;
}

void Character::updateDir(unsigned char key, float timeInterval){
    Point* p = new Point(*getDir());
    cout << key << ":" << GLUT_KEY_RIGHT << ":" << GLUT_KEY_LEFT << endl;
    switch(key){
        case 100: // D
                dir->rotateYaw(this->rotationalVelocity*timeInterval);
            break;
        case 113: // Q
                dir->rotateYaw(-1*(this->rotationalVelocity)*timeInterval);
            break;
        default:
            break;
    }
    delete p;
}

void Character::updateDir(int deltaX, int deltaY, float deltaTime){
    Point* p = new Point(*getDir());
    dir->rotateYaw(this->rotationalVelocity*deltaX*deltaTime);
    dir->rotatePitch(-1*this->rotationalVelocity*deltaY*deltaTime);
    cout << dir->x << endl;
    cout << dir->y << endl;
    cout << dir->z << endl;
}
