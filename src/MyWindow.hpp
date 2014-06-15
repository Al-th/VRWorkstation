#ifndef MYWINDOW_HPP_INCLUDED
#define MYWINDOW_HPP_INCLUDED

#include <windows.h>
#include <iostream>
#include <list>
#include <math.h>
#include "Point.hpp"

using namespace std;

class MyWindow{
public :
    MyWindow(char*, HWND, int, RECT, MyWindow*);
    ~MyWindow();

    void setFullScreen();
    void findAllChilds();
    HBITMAP getHBitmap();
    bool convertToDib(HBITMAP &hBitmap);
    void setConfiguration(int);
    void adjustConfiguration(RECT*,float);

    MyWindow* previousWindow;
    char* windowName;
    HWND hwnd;
    int winID;
    RECT winPos;
    float rotation;
    float leftZOrder;
    float rightZOrder;
    list<MyWindow*> childList;
    MyWindow* parent;

    private:

};

#endif // MYWINDOW_HPP_INCLUDED
