#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>
#include <cstdlib>

#include "Vec3.h"
template <class T> class Vec3;

using namespace std;


template <class T> class Matrix{
    public :
        int m,n; // size matrix
        T** data;

        Matrix<T>(){
            m=3;
            n=3;
            data = (T**)malloc(m*sizeof(T*));
            for(int i = 0; i < m; i++){
                data[i] = (T*)malloc(n*sizeof(T));
                for(int j = 0; j < n; j++){
                    data[i][j] = 0;
                }
            }
        }

        Matrix<T>(int requestedM, int requestedN){
            if(requestedM < 0 || requestedN < 0){
                cout << "Matrix<T> constructor not allowed with negative sizes, returning 2x2 matrix" << endl;
                m=2;
                n=2;
            }
            else{
                m = requestedM;
                n = requestedN;
            }
            data = (T**)malloc(m*sizeof(T*));
            for(int i = 0; i < m; i++){
                data[i] = (T*)malloc(n*sizeof(T));
                for(int j = 0; j < n; j++){
                    data[i][j] = 0;
                }
            }

        }

        void operator += (Matrix<T> m2){
            if(m==m2.m && n == m2.n){
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n ; j++){
                        data[i][j]+=m2.data[i][j];
                    }
                }
            }
            else{
                 cout << "Matrix<T> += operation not allowed : size mismatch" << endl;
            }
        }

        Matrix<T> operator + (Matrix<T> m2){
            if(m==m2.m && n == m2.n){
                Matrix<T> resMat(m,n);
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n ; j++){
                        resMat.data[i][j] = data[i][j] + m2.data[i][j];
                    }
                }
                return resMat;
            }
            else{
                 cout << "Matrix<T> + operation not allowed : size mismatch" << endl;
                 return Matrix<T>();
            }
        }

        void operator += (T scalar){
            for(int i = 0; i < m; i++){
                for (int j = 0; j < n ; j++){
                    data[i][j]+=scalar;
                }
            }
        }

        Matrix<T> operator + (T scalar){
            Matrix<T> resMat(m,n);
            for(int i = 0; i < m; i++){
                for (int j = 0; j < n ; j++){
                    resMat.data[i][j] = data[i][j] + scalar;
                }
            }
            return resMat;
        }

        void operator -= (Matrix<T> m2){
            if(m==m2.m && n == m2.n){
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n ; j++){
                        data[i][j]-=m2.data[i][j];
                    }
                }
            }
            else{
                 cout << "Matrix<T> -= operation not allowed : size mismatch" << endl;
            }
        }

        Matrix<T> operator - (Matrix<T> m2){
            if(m==m2.m && n == m2.n){
                Matrix<T> resMat(m,n);
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n ; j++){
                        resMat.data[i][j]=data[i][j]-m2.data[i][j];
                    }
                }
            }
            else{
                 cout << "Matrix<T> -= operation not allowed : size mismatch" << endl;
                 return Matrix<T>();
            }
        }

        void operator -= (T scalar){
            for(int i = 0; i < m; i++){
                for (int j = 0; j < n ; j++){
                    data[i][j]-=scalar;
                }
            }
        }

        Matrix<T> operator - (T scalar){
            Matrix<T> resMat(m,n);
            for(int i = 0; i < m; i++){
                for (int j = 0; j < n ; j++){
                    resMat.data[i][j] = data[i][j] - scalar;
                }
            }
            return resMat;
        }

        void operator *= (T scalar){
            for(int i = 0; i < m; i++){
                for(int j = 0; j < n ; j++){
                    data[i][j]*=scalar;
                }
            }
        }

        Matrix<T> operator * (T scalar){
            Matrix<T> resMat(m,n);
            for(int i = 0; i < m; i++){
                for (int j = 0; j < n ; j++){
                    resMat.data[i][j] = data[i][j] * scalar;
                }
            }
            return resMat;
        }


        void operator /= (T scalar){
            if(scalar!=0){
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n ; j++){
                        data[i][j]/=scalar;
                    }
                }
            }
            else{
                cout << "Matrix<T> /= operation not allowed : scalar is equal to 0" << endl;
            }
        }

        Matrix<T> operator / (T scalar){
            if(scalar!=0){
                Matrix<T> resMat(m,n);
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n ; j++){
                        resMat.data[i][j] = data[i][j]/scalar;
                    }
                }
                return resMat;
            }
            else{
                cout << "Matrix<T> /= operation not allowed : scalar is equal to 0" << endl;
                return Matrix<T>();
            }
        }



        Matrix<T> operator * (Matrix<T> m2){
            if(n == m2.m){
                Matrix<T> resMat(m,m2.n);
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < m2.n; j++){
                        for(int k = 0; k < n; k++){
                            resMat.data[i][j] += data[i][k]*m2.data[k][j];
                        }
                    }
                }
                return resMat;
            }
            else{
                cout << "Matrix<T> * operation not allowed : M1.n != M2.m" << endl;
                return Matrix<T>();
            }
        }

        Matrix<T> transpose(){
            Matrix<T> resMat(n,m);

            for(int i = 0; i < m;  i++){
                for(int j = 0; j < n ; j++){
                    resMat.data[j][i] = data[i][j];
                }
            }

            return resMat;
        }

        Vec3<T> operator * (Vec3<T> vec){
            if(m != 3 || n != 3){
                cout << "Matrix<T> * operation not allowed : Matrix.m != 3 or Matrix.n != 3" << endl;
                return Vec3<T>();
            }
            else{
                T newX = data[0][0]*vec.x + data[0][1]*vec.y + data[0][2]*vec.z;
                T newY = data[1][0]*vec.x + data[1][1]*vec.y + data[1][2]*vec.z;
                T newZ = data[2][0]*vec.x + data[2][1]*vec.y + data[2][2]*vec.z;
                return Vec3<T>(newX, newY, newZ);
            }
        }

        void operator * (Vec3<T> *vec){
            if(m != 3 || n != 3){
                cout << "Matrix<T> * operation not allowed : Matrix.m != 3 or Matrix.n != 3" << endl;
            }
            else{
                T newX = data[0][0] * vec->x + data[0][1] * vec->y + data[0][2] * vec->z;
                T newY = data[1][0] * vec->x + data[1][1] * vec->y + data[1][2] * vec->z;
                T newZ = data[2][0] * vec->x + data[2][1] * vec->y + data[2][2] * vec->z;
                vec->x = newX;
                vec->y = newY;
                vec->z = newZ;
            }
        }



        void print(){
            cout << endl;
            cout << "------------" << endl;
            cout << "Matrix " << m <<"x"<<n<< " of type " << typeid(T).name() << endl;
            cout << endl;
            for(int i = 0; i < m; i++){
                for(int j = 0; j < n ; j++){
                    cout <<  data[i][j] << " ";
                }
                cout << endl;
            }
            cout << "------------" << endl;

        }

    private :


};
#endif // MATRIX_H_INCLUDED
