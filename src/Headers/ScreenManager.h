#ifndef SCREENMANAGER_H_INCLUDED
#define SCREENMANAGER_H_INCLUDED

#include "Window.h"
#include "Screen.h"
#include "CullingEngine.h"
#include <cmath>

using namespace std;

class ScreenManager{
    public:
        ScreenManager(int);
        ~ScreenManager();

        void bindWindowToScreen(Window*, int);
        Vec3<double>** computeCorners(int, int);

        void renderFunction(CullingEngine*);
        void idleFunction(CullingEngine*);
    private:
        int nbScreens;
        Screen** screenPool;
};

#endif // SCREENMANAGER_H_INCLUDED
