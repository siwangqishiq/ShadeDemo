//
// Created by Administrator on 2020/10/9.
//

#ifndef SHADEDEMO_IMAGERENDER_H
#define SHADEDEMO_IMAGERENDER_H

#include "Log.h"
#include "Util.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

class ImageRender {
public:
    ImageRender(){}
    ~ImageRender(){}

    void init();

    void resize(int width , int height);

    void render();

    void free();

private:
    GLuint mProgramId;
    GLuint mBufferId;
    GLuint textureId;

    float vertexData[4 * 4] = {
            -1.0f , -1.0f ,0.0f , 1.0f,
            -1.0f , 1.0f , 0.0f , 0.0f,
            1.0f , 1.0f,  1.0f , 0.0f,
            1.0f , -1.0f, 1.0f , 1.0f
    };
};


#endif //SHADEDEMO_IMAGERENDER_H
