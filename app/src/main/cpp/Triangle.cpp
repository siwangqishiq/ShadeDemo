//
// Created by  潘易  on 20/10/7.
//

#include "Triangle.h"


void Triangle::init() {
    this->mProgramId = loadShaderFromAssets("simple_vert.glsl","simple_frag.glsl");

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    this->mBufferId = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , mBufferId);
    glBufferData(GL_ARRAY_BUFFER ,  3 * 2 * sizeof(float) , this->vertexData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void Triangle::render() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgramId);
    glBindBuffer(GL_ARRAY_BUFFER , this->mBufferId);
    glVertexAttribPointer(0 , 2 , GL_FLOAT , false , 2 * sizeof(float) , 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES , 0 , 3);

    glDisableVertexAttribArray(0);
}

void Triangle::resize(int w, int h) {
    this->init();

    glViewport(0 ,0 , w , h);
}

void Triangle::free() {
    GLuint bufferIds[1];
    bufferIds[0] = this->mBufferId;
    glDeleteBuffers(1 , bufferIds);
}


