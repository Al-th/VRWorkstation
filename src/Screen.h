#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <windows.h>
#include <gl/glut.h>
#include "Window.h"
#include "Utils\Vec3.h"

using namespace std;

class Screen{
    public:
        Screen(int, Vec3<double>**);
        ~Screen();

        int getScreenID();

        void bindWindow(Window*);
        void drawWindow(Window*, float);

        void renderFunction();
        void idleFunction();
    private:
        Window** anchorWindow;
        Vec3<double>** screenCorners;
        int screenID;
        int nbWindows;

        void renderScreen();
        void renderWindows();
        GLuint screenTexture;
};


#endif // SCREEN_H_INCLUDED
