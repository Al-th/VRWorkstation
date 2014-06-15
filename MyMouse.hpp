#ifndef MYMOUSE_HPP_INCLUDED
#define MYMOUSE_HPP_INCLUDED


#include <windows.h>
#include <iostream>

using namespace std;

class MyMouse{
    public:
        MyMouse();
        ~MyMouse();

        HBITMAP getMouseBitmap();
        bool convertToDib(HBITMAP &hBitmap);
    private:


};

#endif // MYMOUSE_HPP_INCLUDED
