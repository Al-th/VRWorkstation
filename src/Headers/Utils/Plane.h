#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "Vec3.h"

template <class T> class Vec3;

using namespace std;

template <class T> class Plane{
    public :
        Vec3<T> normal;
        T d;

        Plane(){
        }

        Plane(Vec3<T> p1, Vec3<T> p2, Vec3<T> p3){
            Vec3<T> v1 = p2-p1;
            Vec3<T> v2 = p3-p1;
            normal = v1.crossProduct(v2);
            normal.normalize();
            d = -(normal.dotProduct(p1));
        }

        Plane(Vec3<T> origin, Vec3<T> normal){
            this->normal = normal;
            d = -(normal.dotProduct(origin));
        }

        void update(Vec3<T> origin, Vec3<T> normal){
            this->normal = normal;
            d = -(normal.dotProduct(origin));
        }

        T distanceFromPoint(Vec3<T> p){
            T dist = (normal.dotProduct(p) + d);
            return dist;
        }

        void print(){
            cout << endl;
            cout << "----------" << endl;
            cout << "Plane of type " << typeid(T).name() << endl;
            cout << endl;
            cout << "Normal is : "<< endl;
            normal.print();
            cout << "d = " << d;
            cout << "----------" << endl;
        }


};



#endif // PLANE_H_INCLUDED
