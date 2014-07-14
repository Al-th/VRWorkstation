#include "Screen.h"



Screen::Screen(int screenID, Vec3<double>** screenCorners){
    this->screenID = screenID;
    this->screenCorners = screenCorners;

    this->nbWindows = 0;
    anchorWindow = (Window**)malloc(sizeof(Window*));

    glGenTextures(1,&screenTexture);
}

Screen::~Screen(){
    for(int i = 0; i < nbWindows; i++){
        free(anchorWindow[i]);
    }
    free(anchorWindow);

    for(int i = 0 ; i < 4; i++){
        free(screenCorners[i]);
    }
    free(screenCorners);
}

void Screen::bindWindow(Window* window){
    anchorWindow[nbWindows] = window;
    nbWindows += 1;
    anchorWindow = (Window**) realloc(anchorWindow, (nbWindows+1)*sizeof(Window*));
}

int Screen::getScreenID(){
    return screenID;
}

void Screen::idleFunction(){
 for (int i = 0 ; i < nbWindows; i++){
        Window* currentWindow = anchorWindow[i];
        currentWindow->updateContext();
    }
}

void Screen::renderScreen(){
    glBegin(GL_QUAD_STRIP);
    glColor3f(1,1,1);

    float left = screenCorners[0]->x;
    float right = screenCorners[2]->x;
    float top = screenCorners[0]->y;
    float bottom = screenCorners[1]->y;
    float leftZ = -screenCorners[0]->z;
    float rightZ = -screenCorners[2]->z;

    glVertex3f(left,top,leftZ);
    glVertex3f(right, top, rightZ);
    glVertex3f(left,bottom, leftZ);
    glVertex3f(right, bottom, rightZ);

    glEnd();
}

void Screen::drawWindow(Window* currentWindow, float zOrder){
    HWND windowHandle = currentWindow->getHwnd();
    DWORD winStyle = GetWindowLong(windowHandle, GWL_STYLE);
    if(IsWindowVisible(windowHandle)){
        GLuint screenTexture;
        glBindTexture(GL_TEXTURE_2D, screenTexture);

        BITMAP bitmap = currentWindow->getBitmap();
        // Bitmap failed to be captured
        if(bitmap.bmBits == NULL){
            return;
        }

        int data_size = bitmap.bmWidth * bitmap.bmHeight * 4;


        bool isFullyBlack = 1;
        for (int i=0; i<data_size; i+=4){
            // Can remove this with BGRA instead of RGBA ?
            unsigned char tmp = ((unsigned char*)bitmap.bmBits)[i];
            ((unsigned char*)bitmap.bmBits)[i] = ((unsigned char*)bitmap.bmBits)[i+2];
            ((unsigned char*)bitmap.bmBits)[i+2] = tmp;

            unsigned char* vals = (unsigned char*) bitmap.bmBits;
            if(vals[0]!=0 && vals[1]!=0 && vals[2]!=0){
                isFullyBlack = 0;
            }
        }

        if(isFullyBlack){
            return;
        }

        glTexImage2D(GL_TEXTURE_2D,0,4,bitmap.bmWidth,bitmap.bmHeight,0,GL_RGBA, GL_UNSIGNED_BYTE, bitmap.bmBits);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



        glColor4f(1,1,1,1);

        Vec3<double>** windowCorners = (Vec3<double>**)malloc(4*sizeof(Vec3<double>*));
        for(int i = 0 ; i < 4 ; i++){
            windowCorners[i] = new Vec3<double>(*screenCorners[i]);
            *windowCorners[i] *= zOrder;
        }

        float left = windowCorners[0]->x;
        float right = windowCorners[2]->x;
        float top = windowCorners[0]->y;
        float bottom = windowCorners[1]->y;
        float leftZ = -windowCorners[0]->z;
        float rightZ = -windowCorners[2]->z;

        glBegin(GL_TRIANGLE_STRIP);

        glTexCoord2f(0,0);
        glVertex3f(left,top,leftZ);
        glTexCoord2f(1,0);
        glVertex3f(right, top,rightZ );
        glTexCoord2f(0,1);
        glVertex3f(left, bottom,leftZ);
        glTexCoord2f(1,1);
        glVertex3f(right, bottom,rightZ);

        glEnd();
    }
    for(int i = 0; i < currentWindow->childList.size();i++){
        Window* child = currentWindow->childList.front();
        currentWindow->childList.pop_front();
        float deltaZOrder = (float)(currentWindow->childList.size()-i)/(currentWindow->childList.size()*10000);
        drawWindow(child,1+deltaZOrder);
        currentWindow->childList.push_back(child);

    }
}

void Screen::renderWindows(){
    glEnable(GL_TEXTURE_2D);
    for (int i = 0 ; i < nbWindows; i++){
        Window* currentWindow = anchorWindow[i];
        drawWindow(currentWindow,1);

    }
    glDisable(GL_TEXTURE_2D);
}

void Screen::renderFunction(){
    renderScreen();
    renderWindows();
}
