#ifndef Character_HPP_INCLUDED
#define Character_HPP_INCLUDED

#include "Point.hpp"
#include <iostream>

using namespace std;

class Character{
    public :

        Character();
        ~Character();

        Point* getPos();
        Point* getDir();
        Point* getUp();
        void updatePos(unsigned char, float);
        void updateDir(unsigned char, float);
        void updateDir(int,int,float);

        float rotationalVelocity;
        float translationalVelocity;

    private :
        Point* pos;
        Point* dir;
        Point* up;



};



#endif
