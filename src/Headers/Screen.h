#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <windows.h>

#include <GL/glew.h>
#include <gl/glut.h>
#include "Window.h"
#include "Utils/Vec3.h"
#include "CullingEngine.h"

class CullingEngine;

using namespace std;

class Screen{
    public:
        Screen(int, Vec3<double>**);
        ~Screen();

        int getScreenID();
        Vec3<double>** getCorners();

        void bindWindow(Window*);
        void drawWindow(Window*, float);

        void renderFunction(CullingEngine*);
        void idleFunction(CullingEngine*);
    private:
        Window** anchorWindow;
        Vec3<double>** screenCorners;
        int screenID;
        int nbWindows;

        void renderScreen(int);
        void renderWindows();
        GLuint screenTexture;
};


#endif // SCREEN_H_INCLUDED
