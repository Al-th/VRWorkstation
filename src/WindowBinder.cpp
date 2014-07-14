#include "Headers/WindowBinder.h"


WindowBinder::WindowBinder(ScreenManager* screenManager){
    this->screenManager = screenManager;
}

WindowBinder::~WindowBinder(){

}


bool CALLBACK EnumWindowsProc(HWND hWnd, LPARAM param)
{

    WindowBinder* binder = (WindowBinder*)param;
    char WindowName[80];
    GetWindowText(hWnd, WindowName, 80);

    if(strlen(WindowName)>0){
        if(!strcmp(WindowName,binder->requestedName)){
            binder->finalizeBinding(hWnd,binder->requestedScreen);
        }
    }
    return true;
}

void WindowBinder::bindWindow(char* windowName, int screenNb){
    requestedName = windowName;
    requestedScreen = screenNb;

    EnumDesktopWindows(NULL, (WNDENUMPROC)EnumWindowsProc,(LPARAM)this);
}

void WindowBinder::finalizeBinding(HWND hWnd, int screenNb){
    Window* window = new Window(hWnd);
    screenManager->bindWindowToScreen(window, screenNb);
}

