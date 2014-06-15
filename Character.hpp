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
        void updatePos(unsigned char);
        void updateDir(unsigned char);

        float rotationalVelocity;

    private :
        Point* pos;
        Point* dir;
        Point* up;



};



#endif
