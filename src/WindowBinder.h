#ifndef WINDOWBINDER_H_INCLUDED
#define WINDOWBINDER_H_INCLUDED

#include "Windows.h"
#include "Window.h"
#include "ScreenManager.h"

using namespace std;

class WindowBinder{
    public :
        WindowBinder(ScreenManager*);
        ~WindowBinder();

        void bindWindow(char*, int);
        void finalizeBinding(HWND, int);


        char* requestedName;
        int requestedScreen;
    private :
        ScreenManager* screenManager;


};

#endif // WINDOWBINDER_H_INCLUDED
