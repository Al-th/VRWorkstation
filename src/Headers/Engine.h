#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <windows.h>
#include <gl/glut.h>
#include <Math.h>

#include "ScreenManager.h"
#include "WindowBinder.h"
#include "Character.h"



class Engine{
    public :
        Engine(int*, char**);
        ~Engine();

        void initOpenGL(int*, char**);

        void keyboardCallback(unsigned char, int, int);
        void mouseCallback(int,int);
        void renderCallback();
        void reshapeCallback(int, int);
        void idleCallback();

    private :
        ScreenManager* screenManager;
        WindowBinder* windowBinder;
        Character* character;
        float timeInterval;

        void bindWindow(char*,int);

        void lookAt();

        void renderEnvironment();
        void renderRoom();
        void renderOrigin();
};

#endif // ENGINE_H_INCLUDED
