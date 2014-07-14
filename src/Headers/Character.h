#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "Utils\Vec3.h"

using namespace std;

class Character{
    public :

        Character();
        ~Character();

        Vec3<double> getPos();
        Vec3<double> getDir();
        Vec3<double> getUp();

        void mouseFunction(int,int,float);
        void keyboardFunction(unsigned char, float);

        void updatePos(unsigned char, float);
        void updateDir(int,int,float);

    private :
        float rotationalVelocity;
        float velocity;

        Vec3<double> pos;
        Vec3<double> dir;
        Vec3<double> up;
};




#endif // CHARACTER_H_INCLUDED
