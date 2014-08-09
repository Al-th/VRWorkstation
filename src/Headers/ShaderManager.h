#ifndef SHADERMANAGER_H_INCLUDED
#define SHADERMANAGER_H_INCLUDED

#include <fstream>
#include <GL/glew.h>
#include <gl/glut.h>

using namespace std;

class ShaderManager{
    public :
        ShaderManager();
        ~ShaderManager();

        static int getShaderFileLenght(ifstream &file);
        static int loadShader(char* fileName, GLubyte** ShaderSource, GLint* len);
        static int unloadShader(GLubyte** ShaderSource);

};
#endif // SHADERMANAGER_H_INCLUDED
