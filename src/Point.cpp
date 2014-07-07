#include "Point.hpp";

Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::~Point(){
}


void Point::add(Point* p){
    x += p->x;
    y += p->y;
    z += p->z;
}


void Point::substract(Point* p){
    x -= p->x;
    y -= p->y;
    z -= p->z;
}

void Point::multiply(float scalar){
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Point::rotate(float rotationInDeg){

    float pi = atan(1)*4;
    float rotationInRad = rotationInDeg * (pi/180);

    float newX = cos(rotationInRad)*x - sin(rotationInRad)*z;
    float newZ = sin(rotationInRad)*x  + cos(rotationInRad)*z;

    x = newX;
    z = newZ;


}
