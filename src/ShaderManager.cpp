#include "Headers/ShaderManager.h"

int ShaderManager::getShaderFileLenght(ifstream &file){
    if(!file.good()){
        return 0;
    }
    else{
        unsigned long pos = file.tellg();
        file.seekg(0,ios::end);
        unsigned long len = file.tellg();
        file.seekg(ios::beg);
        return len;
    }

}


int ShaderManager::loadShader(char* fileName, GLubyte** shaderSource, GLint* len){
    ifstream file;
    file.open(fileName, ios::in);
    *len = getShaderFileLenght(file);

    if(!file){
        return -1;
    }

    if(len==0){
        return -2;
    }

    GLubyte* tmpShaderSource = (GLubyte*) new char[*len+1];

    for(int i = 0; i < *len+1 ; i++){
        tmpShaderSource[i]=0;
    }
    if(tmpShaderSource == 0 ){
        return -3;
    }

    tmpShaderSource[*len]=0;

    unsigned int i = 0;
    while(file.good()){
        tmpShaderSource[i] = file.get();
        if(!file.eof()){
            i++;
        }
    }

    tmpShaderSource[i] = 0;

    file.close();

    *shaderSource = tmpShaderSource;

    return 0;
}

int ShaderManager::unloadShader(GLubyte** ShaderSource){
    if(*ShaderSource != 0){
        delete [] *ShaderSource;
    }

    *ShaderSource = 0;
}
