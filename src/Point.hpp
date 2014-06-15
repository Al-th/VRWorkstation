#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED

#include <GL/glut.h>
#include <math.h>

class Point {

public:
	Point(float x, float y, float z);
	~Point();

    void add(Point*);
    void substract(Point*);
    void rotate(float);

public:

	float x, y, z;

};



#endif // POINT_HPP_INCLUDED
