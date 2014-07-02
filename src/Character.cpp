#include "Character.hpp"


Character::Character(){
    pos = new Point(0,0,0);
    dir = new Point(0,0,-20);
    up = new Point(0,1,0);
    rotationalVelocity = 1;
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

void Character::updatePos(unsigned char key){
    Point* p = getDir();
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
}

void Character::updateDir(unsigned char key){
    Point* p = getDir();
    cout << key << ":" << GLUT_KEY_RIGHT << ":" << GLUT_KEY_LEFT << endl;
    switch(key){
        case 100: // D
                dir->rotate(this->rotationalVelocity);
            break;
        case 113: // Q
                dir->rotate(-1*(this->rotationalVelocity));
            break;
        default:
            break;
    }
}
