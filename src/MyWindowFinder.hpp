#ifndef MyWindowFinder_HPP_INCLUDED
#define MyWindowFinder_HPP_INCLUDED

#include <windows.h>
#include <iostream>
#include "MyWindow.hpp"

using namespace std;

class MyWindowFinder{
public :
    MyWindowFinder();
    ~MyWindowFinder();

    HBITMAP getDesktopHBitmap();


    void startFetchingNewWindow(char*);
    int getNbWindows();

    char* getLastWindowNameRequest();
    void setLastWindowNameRequest(char*);

    void setNewWindow(char*,HWND);

    void setFullScreen(int);

    MyWindow* getAnchorWindow(int);
private :
    MyWindow** anchorWindow;
    int nbWindows;
    char* lastWindowName;
};

#endif // MyWindowFinder_HPP_INCLUDED
