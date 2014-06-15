#include <windows.h>
#include <gl/glut.h>
#include <Math.h>


#include "Point.hpp"
#include "MyWindowFinder.hpp"
#include "MyMouse.hpp"
#include "Character.hpp"
#include <time.h>

MyWindowFinder* myFinder;
MyMouse* myMouse;

int WindowName;


GLuint screenTexture;
GLuint mouseTexture;

Character* character;

void drawWindow(MyWindow* currentWindow, float zOrder){
    DWORD winStyle = GetWindowLong(currentWindow->hwnd, GWL_STYLE);



    if(IsWindowVisible(currentWindow->hwnd)){
        BITMAP bitmap;
        HBITMAP hbmp;
        //find all childs and sort them according to z-order
        //draw all window on mainWindowpos + relative coord
        //myFinder->getAnchorWindow()->draw();

        glBindTexture(GL_TEXTURE_2D, screenTexture);

        hbmp = currentWindow->getHBitmap();
        currentWindow->convertToDib(hbmp);
        GetObject (hbmp, sizeof(BITMAP), &bitmap);
        int data_size = bitmap.bmWidth * bitmap.bmHeight * 4;

        //RGB to BGR (ou l'autre, j'en ai plus aucune idée)
        bool isFullyBlack = 1;
        for (int i=0; i<data_size; i+=4){
            // Bitmap failed to be captured
            if(bitmap.bmBits == NULL){
                return;
            }
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //Draw texture
        int width = bitmap.bmWidth;
        int height = bitmap.bmHeight;

        glBegin(GL_TRIANGLE_STRIP);

        glColor4f(1,1,1,1);
        float left,right;
        float bottom,top;


        RECT position;
        GetWindowRect(currentWindow->hwnd, &position);

        float scale = 50;



        if(currentWindow->parent!=NULL){
            MapWindowPoints(HWND_DESKTOP, currentWindow->parent->hwnd, (LPPOINT) &position, 2);
            position.left += currentWindow->parent->winPos.left;
            position.top += currentWindow->parent->winPos.top;
            position.right +=  currentWindow->parent->winPos.left;
            position.bottom += currentWindow->parent->winPos.top;

        }
        else{
            position.top -= (float)GetSystemMetrics(SM_CYSCREEN)/2  ;
            position.bottom -= (float)GetSystemMetrics(SM_CYSCREEN)/2;
            position.left -= (float)GetSystemMetrics(SM_CXSCREEN)/2 ;
            position.right -= (float)GetSystemMetrics(SM_CXSCREEN)/2 ;
            currentWindow->adjustConfiguration(&position, scale);
        }



        currentWindow->winPos = position;

        left = position.left;
        right = position.right;
        top = -position.top;
        bottom = top-(float)height;



        top/=scale;
        left/=scale;
        bottom/=scale;
        right/=scale;

        glTexCoord2f(0,0);
        glVertex3f(left,bottom,currentWindow->leftZOrder);
        glTexCoord2f(1,0);
        glVertex3f(right, bottom,currentWindow->rightZOrder);
        glTexCoord2f(0,1);
        glVertex3f(left, top,currentWindow->leftZOrder);
        glTexCoord2f(1,1);
        glVertex3f(right, top,currentWindow->rightZOrder);

        glEnd();

        DeleteObject(hbmp);
        }

        for(int i = 0; i < currentWindow->childList.size();i++){
            MyWindow* child = currentWindow->childList.front();
            currentWindow->childList.pop_front();
            float deltaZOrder = (float)(currentWindow->childList.size()-i)/(currentWindow->childList.size()*10000);
            drawWindow(child,zOrder+deltaZOrder);
            currentWindow->childList.push_back(child);

        }
}


void drawScreenSurface(){
    //TODO : WindowPosition for mouse()

    for (int i = 0 ; i < myFinder->getNbWindows(); i++){
        MyWindow* currentWindow = myFinder->getAnchorWindow(i);
        currentWindow->setConfiguration(i);
        if(currentWindow!=NULL){
            currentWindow->findAllChilds();
            drawWindow(currentWindow,0);
        }
    }
}

void drawOrigin(){
    glBegin(GL_LINES);

    glColor4f(1,1,1,1);
    glVertex3i(0,0,0);glVertex3i(1,0,0);
    glVertex3i(0,0,0);glVertex3i(0,1,0);
    glVertex3i(0,0,0);glVertex3i(0,0,1);

    glEnd();
}


void lookAt(Point* pos, Point* dir, Point* top){
     gluLookAt(pos->x,pos->y,pos->z,pos->x+dir->x,pos->y+dir->y,pos->z+dir->z,top->x,top->y,top->z);
}

void drawMouse(){

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Get mouse and set texture
    BITMAP bitmap;
    HBITMAP hbmp;

    glBindTexture(GL_TEXTURE_2D, mouseTexture);
    hbmp = myMouse->getMouseBitmap();
    myMouse->convertToDib(hbmp);

    GetObject (hbmp, sizeof(BITMAP), &bitmap);
    int data_size = bitmap.bmWidth * bitmap.bmHeight * 4;


    for (int i=0; i<data_size; i+=4){
        unsigned char tmp = ((unsigned char*)bitmap.bmBits)[i];
        ((unsigned char*)bitmap.bmBits)[i] = ((unsigned char*)bitmap.bmBits)[i+2];
        ((unsigned char*)bitmap.bmBits)[i+2] = tmp;
    }

    glTexImage2D(GL_TEXTURE_2D,0,4,bitmap.bmWidth,bitmap.bmHeight,0,GL_RGBA, GL_UNSIGNED_BYTE, bitmap.bmBits);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //Draw texture
    int width = bitmap.bmWidth;
    int height = bitmap.bmHeight;

    glBegin(GL_TRIANGLE_STRIP);

    glColor4f(1,1,1,1);
    float left,right;
    float bottom,top;



    POINT cursorPosition;
    GetCursorPos(&cursorPosition);
    if(myFinder->getAnchorWindow(0)!= NULL && ScreenToClient(myFinder->getAnchorWindow(0)->hwnd, &cursorPosition)){
        float width = GetSystemMetrics(SM_CXSCREEN);
        float height = GetSystemMetrics(SM_CYSCREEN);

        left = (float)myFinder->getAnchorWindow(0)->winPos.left/50. + (float)cursorPosition.x/50;
        right = left + (float)width/256;
        top = -(float)myFinder->getAnchorWindow(0)->winPos.top/50.  - (float)cursorPosition.y /50;
        bottom = top - (float)height/256;



        glTexCoord2f(0,0);
        glVertex3f(left,bottom,0.001);
        glTexCoord2f(1,0);
        glVertex3f(right, bottom,0.001);
        glTexCoord2f(0,1);
        glVertex3f(left, top,0.001);
        glTexCoord2f(1,1);
        glVertex3f(right, top,0.001);
        glEnd();

    }

    glDisable(GL_BLEND);
    DeleteObject(hbmp);
}


void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	//Efface le frame buffer et le Z-buffer
    glMatrixMode(GL_MODELVIEW);	//Choisit la matrice MODELVIEW
    glLoadIdentity();	//Réinitialise la matrice


    lookAt(character->getPos(),character->getDir(),character->getUp());


    drawScreenSurface();
    drawOrigin();
    drawMouse();

    glutSwapBuffers();
    glutPostRedisplay();

}


void reshape(int width, int height){
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)width/(float)height;
    gluPerspective(45, ratio, 0.1, 1000);	//Pour les explications, lire le tutorial sur OGL et win
    glMatrixMode(GL_MODELVIEW);	//Optionnel
}

void keyboardFunc(unsigned char key, int x, int y){
    character->updatePos(key);
    character->updateDir(key);

}

void mouseFunc(int button, int state, int x, int y){
    //
}


void initGL(){
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1,&screenTexture);
    glGenTextures(1,&mouseTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glutSetCursor(GLUT_CURSOR_NONE);

}

int main( int argc, char *argv[ ], char *envp[ ] ){

    myFinder = new MyWindowFinder();
    myMouse = new MyMouse();

    myFinder->startFetchingNewWindow("Sans titre - Paint");
    myFinder->startFetchingNewWindow("SoundWire Server");
    myFinder->startFetchingNewWindow("Sans titre - Paint");
    for (int i = 0 ; i < myFinder->getNbWindows();i++){
        myFinder->setFullScreen(i);
    }

    srand(time(NULL));

    //Initialisation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024,800);	//Optionnel
    WindowName = glutCreateWindow("VRWorkstation");
    //glutFullScreen();

    //Callbacks();
    glutReshapeFunc(reshape);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);


    initGL();

    character = new Character();
    glutMainLoop();
    return 0;

}
