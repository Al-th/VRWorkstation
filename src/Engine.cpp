#include "Headers/Engine.h"

#define NB_SCREENS 8
#define OPENGL_WINDOW_WIDTH 1024
#define OPENGL_WINDOW_HEIGHT 800

Engine::Engine(int* argc, char** argv){
    screenManager = new ScreenManager(NB_SCREENS);
    windowBinder = new WindowBinder(screenManager);
    character = new Character();
    cullingEngine = new CullingEngine((float)0.1,(float)1000,(float)OPENGL_WINDOW_WIDTH/(float)OPENGL_WINDOW_HEIGHT,(float)45,
                                      character->getPos(),character->getDir(), character->getUp());
    timeInterval = 0;

    initOpenGL(argc, argv);

    for(int i = 0 ; i < 8 ; i++){
        bindWindow("The Idiot Test Challenge - YouTube - Google Chrome",i);
    }
    bindWindow("Google - Google Chrome",5);

}

Engine::~Engine(){
    delete screenManager;
    delete windowBinder;
    delete character;
}

void Engine::initOpenGL(int* argc, char** argv){
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(960,540);	//Optionnel
    glutCreateWindow("VRWorkstation");

    GLenum err = glewInit();
    cout << "Glew Init : " << err << endl;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glutSetCursor(GLUT_CURSOR_NONE);

    //initShaders();
}


void Engine::bindWindow(char* windowName, int screenNumber){
    windowBinder->bindWindow(windowName, screenNumber);
}

/* Callbacks to give to OpenGL functions */
void Engine::mouseCallback(int x, int y){
    if(x!=(OPENGL_WINDOW_WIDTH/2) || y != (OPENGL_WINDOW_HEIGHT/2)){
        float deltaX = x-(OPENGL_WINDOW_WIDTH/2);
        float deltaY = y-(OPENGL_WINDOW_HEIGHT/2);
        float deltaT = timeInterval/1000.0;

        character->mouseFunction(deltaX, deltaY, deltaT);
        glutWarpPointer(OPENGL_WINDOW_WIDTH/2,OPENGL_WINDOW_HEIGHT/2);
    }
}

void Engine::keyboardCallback(unsigned char key, int x, int y){
    character->keyboardFunction(key, timeInterval/1000.0);
}

void Engine::lookAt(int eye){
    Vec3<double> pos = character->getPos();
    Vec3<double> dir = character->getDir();
    Vec3<double> up = character->getUp();

    //TODO adapt to rift for eye ovrHMD
    if(eye==0){
        pos.x-=5;
    }

    gluLookAt(pos.x,pos.y,pos.z,pos.x+dir.x,pos.y+dir.y,pos.z+dir.z,up.x,up.y,up.z);
}

void Engine::renderCallback(){
    float width = 960;
    float height = 540;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	//Efface le frame buffer et le Z-buffer
    for(int eye = 0; eye < 2; eye++){
        glViewport(eye*width/2.0,0,width/2.0,height);
        glMatrixMode(GL_MODELVIEW);	//Choisit la matrice MODELVIEW
        glLoadIdentity();	//Réinitialise la matrice
        lookAt(eye);

        renderEnvironment();
        screenManager->renderFunction(cullingEngine);

    }
    glutSwapBuffers();
    glutPostRedisplay();
}

void Engine::reshapeCallback(int width, int height){
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)width/(float)height;
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);	//Optionnel
}
void Engine::idleCallback(){
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeInterval = currentTime - timeInterval;
    screenManager->idleFunction(cullingEngine);
    cullingEngine->updateOrigin(character->getPos(),character->getDir(), character->getUp());


}

void Engine::renderEnvironment(){
    renderRoom();
    renderOrigin();
}

void Engine::renderOrigin(){
    glBegin(GL_LINES);

    glColor4f(1,1,1,1);
    glVertex3i(0,0,0);glVertex3i(1,0,0);
    glVertex3i(0,0,0);glVertex3i(0,1,0);
    glVertex3i(0,0,0);glVertex3i(0,0,1);

    glEnd();
}

void Engine::renderRoom(){
    float size = 100;

    glBegin(GL_QUADS);
    glColor3f(0,0,1);
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);
    glVertex3f(-size, -size, -size);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1,0,0);
    glVertex3f(-size, size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(-size, -size, -size);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1,1,0);
    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
    glVertex3f(-size, -size, size);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0,1,1);
    glVertex3f(size, -size, size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, size, size);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1,0,1);
    glVertex3f(-size, -size,-size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, size, -size);
    glEnd();
}

void Engine::initShaders(){
    GLuint fragmentShaderObject;
    GLuint vertexShaderObject;
    GLubyte* fragmentShaderSource;
    GLubyte* vertexShaderSource;
    char* fragmentShaderFileName = "";
    char* vertexShaderFileName = "";
    GLint flength;
    GLint vlength;
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    ShaderManager::loadShader(fragmentShaderFileName,&fragmentShaderSource,&flength);
    ShaderManager::loadShader(vertexShaderFileName, &vertexShaderSource, &vlength);
    glShaderSourceARB(fragmentShaderObject, 1, (const GLcharARB**) &fragmentShaderSource, &flength);
    glShaderSourceARB(vertexShaderObject, 1, (const GLcharARB**) &vertexShaderSource, &vlength);
    glCompileShaderARB(fragmentShaderObject);
    glCompileShaderARB(vertexShaderObject);

    GLint fcompiled;
    GLint vcompiled;
    glGetObjectParameterivARB(fragmentShaderObject, GL_COMPILE_STATUS, &fcompiled);
    glGetObjectParameterivARB(vertexShaderObject, GL_COMPILE_STATUS, &vcompiled);
    if (vcompiled && fcompiled)
    {
        cout << "Shaders compiled" << endl;
        GLuint programObject = glCreateProgram();
        glAttachShader(programObject, fragmentShaderObject);
        glAttachShader(programObject, vertexShaderObject);
        glLinkProgram(programObject);
        glUseProgram(programObject);
    }
    else{
        cout << "Shader not compiled" << endl;
    }
}

