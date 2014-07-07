#include "MyMouse.hpp"



MyMouse::MyMouse(){
}

MyMouse::~MyMouse(){
}

HBITMAP MyMouse::getMouseBitmap(){
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    // Create the bitmap to use as a canvas.
    HBITMAP mouseBitmap = CreateCompatibleBitmap(hdcScreen, 256,256);

    // Select the bitmap into the device context.


    // Get information about the global cursor.
    CURSORINFO ci;
    ci.cbSize = sizeof(ci);
    GetCursorInfo(&ci);


    // Draw the cursor into the canvas.
    HGDIOBJ hbmOld = SelectObject(hdcMem, mouseBitmap);
    DrawIconEx (hdcMem, 0, 0, ci.hCursor, 0, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), NULL, DI_NORMAL);
    mouseBitmap = (HBITMAP)SelectObject(hdcMem, hbmOld);


    DeleteDC(hdcMem);
    DeleteDC(hdcScreen);

    return mouseBitmap;
}

bool MyMouse::convertToDib(HBITMAP &hBitmap)
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
