#include "MyWindow.hpp"
#include <time.h>

MyWindow::MyWindow(char* winName, HWND winHandle, int winID, RECT position, MyWindow* parent){
    this->windowName = winName;
    this->hwnd = winHandle;
    this->previousWindow = previousWindow;
    this->winID = winID;
    this->winPos = position;
    this->parent = parent;
}




void MyWindow::findAllChilds(){
    while(childList.size()!=0){
        childList.pop_back();
    }
    int i = 0;
    HWND tmpWindowHandle = GetTopWindow(NULL);
    while(tmpWindowHandle!=NULL){
        DWORD pid;
        DWORD currentWindowPid;
        GetWindowThreadProcessId(tmpWindowHandle,&pid);
        GetWindowThreadProcessId(this->hwnd,&currentWindowPid);
        if(pid==currentWindowPid && tmpWindowHandle != this->hwnd){
            RECT position;
            MyWindow* child = new MyWindow("child",tmpWindowHandle,i, position, this);




            //child->findAllChilds(); <= Recursivity for later
            childList.push_front(child);
        }
        tmpWindowHandle = GetNextWindow(tmpWindowHandle,GW_HWNDNEXT);
        i++;
    }
}

MyWindow::~MyWindow(){
    free(this->windowName);
}

void MyWindow::setFullScreen(){
    if(hwnd!=NULL){
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
}

void MyWindow::setConfiguration(int i){
    switch(i){
        case 0:
            break;
        case 1:
            rotation = 35;
            break;
        case 2:
            rotation = -35;
            break;
    }
}

void MyWindow::adjustConfiguration(RECT* pos, float scale){

    Point* p1 = new Point(pos->left,0,0);
    Point* p2 = new Point(pos->right,0,0);

    if(rotation>0){
        p1->rotate(rotation);
        p2->rotate(rotation);

        pos->left = 50+(float)GetSystemMetrics(SM_CXSCREEN)/2;
        pos->right = pos->left + (abs(p1->x) + abs(p2->x));

        leftZOrder = 0;
        rightZOrder = (abs(p1->z)+abs(p2->z))/scale;
    }
    else if (rotation < 0){
        p1->rotate(rotation);
        p2->rotate(rotation);


        pos->right = -50-(float)GetSystemMetrics(SM_CXSCREEN)/2;
        pos->left = pos->right - (abs(p1->x) + abs(p2->x));

        leftZOrder = (abs(p1->z)+abs(p2->z))/scale;
        rightZOrder = 0;
    }


}

HBITMAP MyWindow::getHBitmap(){
    HDC hdcCapture;
    HDC hdcMemCapture;
    HBITMAP hBitmap;
    HGDIOBJ oldCaptureBitmap;

    RECT clientRect;
    GetClientRect(hwnd,&clientRect);
    int width = clientRect.right-clientRect.left;
    int height = clientRect.bottom-clientRect.top ;

    hdcCapture = GetDC (hwnd);

    hdcMemCapture = CreateCompatibleDC(hdcCapture);

    hBitmap = CreateCompatibleBitmap(hdcCapture,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top);

    oldCaptureBitmap = SelectObject(hdcMemCapture, hBitmap);

    BitBlt(hdcMemCapture, 0,0, width, height, hdcCapture, 0,0, SRCCOPY);

    hBitmap = (HBITMAP)SelectObject(hdcMemCapture, oldCaptureBitmap);

    DeleteDC(hdcMemCapture);
    DeleteDC(hdcCapture);

    return hBitmap;
}



bool MyWindow::convertToDib(HBITMAP &hBitmap)
{
    bool bConverted = false;
    BITMAP stBitmap;
    if (GetObject(hBitmap, sizeof(stBitmap),
            &stBitmap) && !stBitmap.bmBits)
    {


    HDC hScreen = GetDC(NULL);
    if (hScreen)
    {
      union {
        BITMAPINFO stBitmapInfo;
        BYTE pReserveSpace[sizeof(BITMAPINFO)
                     + 0xFF * sizeof(RGBQUAD)];
      };
      ZeroMemory(pReserveSpace, sizeof(pReserveSpace));
      stBitmapInfo.bmiHeader.biSize = sizeof(stBitmapInfo.bmiHeader);
      stBitmapInfo.bmiHeader.biWidth = stBitmap.bmWidth;
      stBitmapInfo.bmiHeader.biHeight = stBitmap.bmHeight;
      stBitmapInfo.bmiHeader.biPlanes = 1;
      stBitmapInfo.bmiHeader.biBitCount = stBitmap.bmBitsPixel;
      stBitmapInfo.bmiHeader.biCompression = BI_RGB;

      if (stBitmap.bmBitsPixel <= 8)
      {
        stBitmapInfo.bmiHeader.biClrUsed =
                        1 << stBitmap.bmBitsPixel;

      }
      stBitmapInfo.bmiHeader.biClrImportant =
                       stBitmapInfo.bmiHeader.biClrUsed;

      PVOID pBits;
      HBITMAP hDib = CreateDIBSection(hScreen,
        &stBitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

      if (hDib)
      {
        HDC hMemSrc = CreateCompatibleDC(NULL);
        if (hMemSrc)
        {
          HGDIOBJ hOldSrc = SelectObject(hMemSrc, hBitmap);
          if (hOldSrc)
          {
            HDC hMemDst = CreateCompatibleDC(NULL);
            if (hMemDst)
            {
              HGDIOBJ hOldDst = SelectObject(hMemDst, hDib);
              if (hOldDst)
              {
                if (stBitmap.bmBitsPixel <= 8)
                {
                  // take the DFB's palette and set it to our DIB

                  HPALETTE hPalette =
                    (HPALETTE) GetCurrentObject(hMemSrc, OBJ_PAL);
                  if (hPalette)
                  {
                    PALETTEENTRY pPaletteEntries[0x500];
                    UINT nEntries = GetPaletteEntries(hPalette,
                                    0, stBitmapInfo.bmiHeader.biClrUsed,
                                    pPaletteEntries);
                    if (nEntries)
                    {

                      for (UINT nIndex = 0; nIndex < nEntries; nIndex++)
                        pPaletteEntries[nEntries].peFlags = 0;
                      SetDIBColorTable(hMemDst, 0,
                        nEntries, (RGBQUAD*) pPaletteEntries) == nEntries;

                    }
                  }
                }

                if (BitBlt(hMemDst, 0, 0, stBitmap.bmWidth,
                      stBitmap.bmHeight, hMemSrc, 0, 0, SRCCOPY))
                  bConverted = true; // success


                SelectObject(hMemDst, hOldDst);
              }
              DeleteDC(hMemDst);
            }
            SelectObject(hMemSrc, hOldSrc);
          }
          DeleteDC(hMemSrc);
        }

        if (bConverted)
        {
          DeleteObject(hBitmap);

          hBitmap = hDib;
        }
        else
          DeleteObject(hDib);
      }
      ReleaseDC(NULL, hScreen);
    }
    }
    return bConverted;

}

