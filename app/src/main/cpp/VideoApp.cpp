//
// Created by Administrator on 2020/9/22.
//

#include "VideoApp.h"


void VideoApp::init() {
    //wait resize
}

void VideoApp::resize(int w, int h) {
    this->viewWidth = w;
    this->viewHeight = h;

    onGetViewSize();
}

void VideoApp::onGetViewSize() {
    this->mProgramId = loadShaderFromAssets("simple_vert.glsl","simple_frag.glsl");

    LOGI("load program id = %d " , this->mProgramId);

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    this->mBufferId = bufferIds[0];
}

void VideoApp::render() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgramId);

}

void VideoApp::free() {
    glDeleteProgram(mProgramId);
    GLuint bufferIds[1];
    bufferIds[0] = mBufferId;
    glDeleteBuffers(1 , bufferIds);
}