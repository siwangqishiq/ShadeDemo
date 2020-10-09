//
// Created by panyi on 2020/10/9.
//

#include "ImageRender.h"

void ImageRender::resize(int width, int height) {
    this->init();

    glViewport(0 , 0 , width , height);
}

void ImageRender::init() {
    this->mProgramId = loadShaderFromAssets("image_vert.glsl","image_frag.glsl");

    GLuint bufferIds[2];
    glGenBuffers(1 , bufferIds);
    this->mBufferId = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , mBufferId);
    glBufferData(GL_ARRAY_BUFFER ,  4 * 4 * sizeof(float) , this->vertexData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    int image_width , image_height , image_channel;
    unsigned char* image_data = readImage("baokemeng.jpg" , image_width , image_height , image_channel);

    GLuint textureIds[1];
    glGenTextures(1 , textureIds);
    this->textureId = textureIds[0];

    glBindTexture(GL_TEXTURE_2D , this->textureId);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void ImageRender::render() {
    glClearColor(1.0f , 1.0f , 1.0f , 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgramId);
    glBindBuffer(GL_ARRAY_BUFFER , this->mBufferId);
    glVertexAttribPointer(0 , 2 , GL_FLOAT , false , 4 * sizeof(float) , 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 , 2 , GL_FLOAT , false , 4 * sizeof(float) , (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void ImageRender::free() {
    GLuint bufferIds[1];
    bufferIds[0] = this->mBufferId;
    glDeleteBuffers(1 , bufferIds);

    GLuint textureIds[1];
    textureIds[0] = this->textureId;
    glDeleteTextures(1 , textureIds);
}
