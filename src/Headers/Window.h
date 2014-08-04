#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <windows.h>
#include <list>

class CullingEngine;

using namespace std;

class Window{
    public :
        Window(HWND);
        ~Window();
        void updateContext();
        BITMAP getBitmap();

        HWND getHwnd();

        list<Window*> childList;
    private :

        HWND hwnd;

        void updateBitmap();
        void updateChilds();

        HBITMAP hBitmap;
        BITMAP bitmap;
        void getHBitmap();
        bool convertToDib();


};

#endif // WINDOW_H_INCLUDED
