//
// Created by  潘易  on 20/10/7.
//

#ifndef SHADEDEMO_TRIANGLE_H
#define SHADEDEMO_TRIANGLE_H

#include "Log.h"
#include "Util.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>


class Triangle {
public:
    Triangle(){}

    void init();

    void render();

    void resize(int w , int h);

    void free();

    ~Triangle(){}
private:
    GLuint mProgramId;
    GLuint mBufferId;

    float vertexData[3 * 2] = {
            -0.5f , -0.5f ,
            0.5f , -0.5f ,
            0.0f , 0.5f,
    };
};


#endif //SHADEDEMO_TRIANGLE_H
