#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

#include <iostream>
#include <cmath>
#include <typeinfo>

#include "Constant.h"

#include "Matrix.h"
template <class T> class Matrix;


using namespace std;

template <class T> class Vec3{
    public:
        T x,y,z;
        //vector constructor
        Vec3<T>(){
            x = 0;
            y = 0;
            z = 0;
        }
        Vec3<T>(T x,T y, T z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

        //vector operations
        void normalize(){
            float magnitude = sqrt( (x*x) + (y*y) + (z*z) );
            if(magnitude!=0){
                x /= magnitude;
                y /= magnitude;
                z /= magnitude;
            }
            else{
                cout << "Vec3<T> normalize operation not allowed : Magnitude of Vec3 is equal to 0" << endl;
            }
        }


        T dotProduct(Vec3<T> v2){
                return (x*v2.x + y*v2.y + z*v2.z);
        }

        Vec3<T> crossProduct(Vec3<T> v2){
            T newX = y*v2.z - z*v2.y;
            T newY = z*v2.x - x*v2.z;
            T newZ = x*v2.y - y*v2.x;

            return Vec3(newX,newY,newZ);
        }

        T length(){
            return sqrt( (x*x) + (y*y) + (z*z));
        }


        void rotateYaw(T degAngle){
            T radAngle = degAngle*DEG_TO_RAD_FACTOR;

            Matrix<double> rotationMatrix(3,3);
            rotationMatrix.data[0][0] = cos(radAngle);
            rotationMatrix.data[0][1] = -sin(radAngle);
            rotationMatrix.data[1][0] = sin(radAngle);
            rotationMatrix.data[1][1] = cos(radAngle);
            rotationMatrix.data[2][2] = 1;

            rotationMatrix * this;
        }

        void rotatePitch(T degAngle){
            T radAngle = degAngle*DEG_TO_RAD_FACTOR;

            Matrix<double> rotationMatrix(3,3);
            rotationMatrix.data[0][0] = cos(radAngle);
            rotationMatrix.data[0][2] = sin(radAngle);
            rotationMatrix.data[1][1] = 1;
            rotationMatrix.data[2][0] = -sin(radAngle);
            rotationMatrix.data[2][2] = cos(radAngle);

            rotationMatrix * this;
        }

        void rotateRoll(T degAngle){
            T radAngle = degAngle*DEG_TO_RAD_FACTOR;

            Matrix<double> rotationMatrix(3,3);
            rotationMatrix.data[0][0] = 1;
            rotationMatrix.data[0][1] = cos(radAngle);
            rotationMatrix.data[1][2] = -sin(radAngle);
            rotationMatrix.data[2][1] = sin(radAngle);
            rotationMatrix.data[2][2] = cos(radAngle);

            rotationMatrix * this;
        }

        //operator redefinition
        void operator += (Vec3<T> v){
            x += v.x;
            y += v.y;
            z += v.z;
        }

        Vec3<T> operator + (Vec3<T> v2){
            return Vec3(x+v2.x, y+v2.y, z+v2.z);
        }

        void operator -= (Vec3<T> v){
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }

        Vec3<T> operator - (Vec3<T> v2){
            return Vec3(x-v2.x, y-v2.y, z-v2.z);
        }

        void operator *= (Vec3<T> v){
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }

        Vec3<T> operator * (Vec3<T> v2){
            return Vec3(x*v2.x, y*v2.y, z*v2.z);
        }

        void operator *= (float scalar){
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }

        Vec3<T> operator * (float scalar){
            return Vec3(x*scalar, y*scalar, z*scalar);
        }


        void operator /= (Vec3<T> v){
            if(v.x != 0 && v.y != 0 && v.z != 0){
                x /= v.x;
                y /= v.y;
                z /= v.z;
            }
            else{
                cout << "Vec3<T> /= operation not allowed : components of Vec3<T> v is equal to 0" << endl;
            }
        }

        Vec3<T> operator / (Vec3<T> v2){
            if (v2.x != 0 && v2.y != 0 && v2.z != 0){
                return Vec3(x/v2.x, y/v2.y, z/v2.z);
            }
            else{
                cout << "Vec3<T> / operation not allowed : components of Vec3<T> v2 is equal to 0" << endl;
                return Vec3();
            }
        }

        void operator /= (float scalar){
            if(scalar != 0){
                x *= scalar;
                y *= scalar;
                z *= scalar;
            }
            else{
                cout << "Vec3<T> / operation not allowed : scalar is equal to 0" << endl;
            }
        }

        Vec3<T> operator / (float scalar){
            if(scalar != 0){
                return Vec3(x/scalar, y/scalar, z/scalar);
            }
            else{
                cout << "Vec3<T> / operation not allowed : scalar is equal to 0" << endl;
                return Vec3();
            }
        }

        void print(){
            cout << endl;
            cout << "----------" << endl;
            cout << "Vec3 of type " << typeid(T).name() << endl;
            cout << endl;
            cout << "x = " << x << endl;
            cout << "y = " << y << endl;
            cout << "z = " << z << endl;
            cout << "----------" << endl;
        }


    private:


};

#endif // VEC3_H_INCLUDED
