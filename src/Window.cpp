#include "Headers/Window.h"



Window::Window(HWND hwnd){
    this->hwnd = hwnd;
}

Window::~Window(){
    while(childList.size()!=0){
        Window* w = childList.back();
        childList.pop_back();
        delete w;
    }
}

void Window::updateContext(){
    updateChilds();
    updateBitmap();
}

BITMAP Window::getBitmap(){
    return this->bitmap;
}


HWND Window::getHwnd(){
    return hwnd;
}

 void Window::updateChilds(){
    while(childList.size()!=0){
        Window* w = childList.back();
        childList.pop_back();
        delete w;
    }

    HWND tmpWindowHandle = GetTopWindow(NULL);
    while(tmpWindowHandle!=NULL){
        DWORD pid;
        DWORD currentWindowPid;
        GetWindowThreadProcessId(tmpWindowHandle,&pid);
        GetWindowThreadProcessId(this->hwnd,&currentWindowPid);
        if(pid==currentWindowPid && tmpWindowHandle != this->hwnd){
            Window* child = new Window(tmpWindowHandle);
            childList.push_front(child);
        }
        tmpWindowHandle = GetNextWindow(tmpWindowHandle,GW_HWNDNEXT);
    }
 }

#include <time.h>
#include <iostream>
void Window::updateBitmap(){
        getHBitmap();
        convertToDib();
        GetObject (hBitmap, sizeof(BITMAP), &bitmap);
}

void Window::getHBitmap(){
    HDC hdcCapture;
    HDC hdcMemCapture;
    HBITMAP tmpHBitmap;
    HGDIOBJ oldCaptureBitmap;

    RECT clientRect;
    GetClientRect(hwnd,&clientRect);
    int width = clientRect.right-clientRect.left;
    int height = clientRect.bottom-clientRect.top ;
    hdcCapture = GetDC (hwnd);
    hdcMemCapture = CreateCompatibleDC(hdcCapture);
    tmpHBitmap = CreateCompatibleBitmap(hdcCapture,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top);

    oldCaptureBitmap = SelectObject(hdcMemCapture, tmpHBitmap);
    BitBlt(hdcMemCapture, 0,0, width, height, hdcCapture, 0,0, SRCCOPY);
    tmpHBitmap = (HBITMAP)SelectObject(hdcMemCapture, oldCaptureBitmap);

    DeleteDC(hdcMemCapture);
    DeleteDC(hdcCapture);

    DeleteObject(this->hBitmap);
    this->hBitmap = tmpHBitmap;
}

bool Window::convertToDib(){
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
