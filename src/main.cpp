#include <windows.h>
#include <GL/glew.h>
#include <gl/glut.h>
#include "Headers/Engine.h"
#include <Math.h>
#include <ctime>


Engine* engine;



void keyboardCallback(unsigned char key, int x, int y){
    engine->keyboardCallback(key,x,y);
}

void mouseCallback(int x,int y){
    engine->mouseCallback(x,y);
}

void renderCallback(){
    engine->renderCallback();
}

void reshapeCallback(int width, int height){
    engine->reshapeCallback(width,height);
}

void idleCallback(){
    engine->idleCallback();
}

void startOpenGL(){
    glutReshapeFunc(reshapeCallback);
    glutDisplayFunc(renderCallback);
    glutKeyboardFunc(keyboardCallback);
    glutPassiveMotionFunc(mouseCallback);
    glutIdleFunc(idleCallback);

    glutMainLoop();
}

int main(int argc, char** argv){
    srand(time(NULL));

    engine = new Engine(&argc, argv);

    startOpenGL();

    return 0;
}
