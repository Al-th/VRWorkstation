#include "MyWindowFinder.hpp"
#include <stdio.h>


HBITMAP MyWindowFinder::getDesktopHBitmap(){
    HDC hdcCapture;
    HDC hdcMemCapture;
    HBITMAP hBitmap;
    BITMAP bitmap ;
    HGDIOBJ oldCaptureBitmap;

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    hdcCapture = GetDC(NULL);

    hdcMemCapture = CreateCompatibleDC(hdcCapture);

    hBitmap = CreateCompatibleBitmap(hdcCapture,width,height);

    oldCaptureBitmap = SelectObject(hdcMemCapture, hBitmap);

    BitBlt(hdcMemCapture, 0,0, width, height, hdcCapture, 0,0, SRCCOPY);

    hBitmap = (HBITMAP)SelectObject(hdcMemCapture, oldCaptureBitmap);

    DeleteDC(hdcMemCapture);
    DeleteDC(hdcCapture);

    return hBitmap;
}


void MyWindowFinder::setFullScreen(int i){
    anchorWindow[i]->setFullScreen();
}

bool CALLBACK EnumWindowsProc(HWND hWnd, LPARAM param)
{

    MyWindowFinder* myFinder = (MyWindowFinder*)param;
    char WindowName[80];
    GetWindowText(hWnd, WindowName, 80);


    // This commented section retrieves all windows visible and that are on the task bar. Some performances issues.
    // Integrate in some specific method (performance issues may arise when starting to fetch multiple times
    /*if(IsWindowVisible(hWnd) && strlen(WindowName) > 0){
        char pszClassName[64];
        GetClassName(hWnd, pszClassName, 64);
        if((GetWindowLong(hWnd, GWL_STYLE) & WS_EX_APPWINDOW))
        {
            cout << WindowName << endl;
            myFinder->setNewWindow(WindowName, hWnd);

            cout << endl;
            cout << endl;
        }

    }
*/
    if(strlen(WindowName)>0){
        if(!strcmp(WindowName,myFinder->getLastWindowNameRequest())){
            myFinder->setNewWindow(myFinder->getLastWindowNameRequest(), hWnd);
            cout << "Found one window" << "\n";
        }
    }
    return true;
}

void MyWindowFinder::setNewWindow(char* winName ,HWND winHandle){
    RECT pos;
    GetWindowRect(winHandle, &pos);
    MyWindow* newWindow = new MyWindow(winName, winHandle, 1, pos, NULL);
    anchorWindow[nbWindows] = newWindow;
    nbWindows += 1;
    anchorWindow = (MyWindow**) realloc(anchorWindow, (nbWindows+1)*sizeof(MyWindow*));
}

MyWindow* MyWindowFinder::getAnchorWindow(int i){
    return anchorWindow[i];
}

char* MyWindowFinder::getLastWindowNameRequest(){
    return lastWindowName;
}

void MyWindowFinder::setLastWindowNameRequest(char* windowName){

    lastWindowName = windowName;
}


MyWindowFinder::MyWindowFinder(){
    anchorWindow = (MyWindow**)malloc(sizeof(MyWindow*));
    nbWindows = 0;
}

int MyWindowFinder::getNbWindows(){
    return nbWindows;
}

MyWindowFinder::~MyWindowFinder(){
    for (int i = 0; i < nbWindows ; i++){
        free(anchorWindow[i]);
    }
    free(anchorWindow);
}

void MyWindowFinder::startFetchingNewWindow(char* windowName){
    this->setLastWindowNameRequest(windowName);
    EnumDesktopWindows(NULL, (WNDENUMPROC)EnumWindowsProc,(LPARAM)this);
}
