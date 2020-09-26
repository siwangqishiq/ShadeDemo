//
// Created by Administrator on 2020/9/22.
//

#include <unistd.h>
#include <android/surface_texture.h>
#include "VideoApp.h"
#include "Util.h"

void VideoApp::init() {
    //wait resize
}

void VideoApp::resize(int w, int h) {
    this->viewWidth = w;
    this->viewHeight = h;

    onGetViewSize();
    glViewport(0 , 0 , viewWidth , viewHeight);
}

void VideoApp::onGetViewSize() {
    this->mProgramId = loadShaderFromAssets("simple_vert.glsl","simple_frag.glsl");
    LOGI("load mProgramId id = %d " , this->mRenderVideoProgramId);
    this->mRenderVideoProgramId = loadShaderFromAssets("video_vert.glsl","video_frag.glsl");
    LOGI("load mRenderVideoProgramId id = %d " , this->mRenderVideoProgramId);

    this->mUniformSTMatrixLoc = glGetUniformLocation(this->mRenderVideoProgramId , "uSTMatrix");

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    this->mBufferId = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , mBufferId);
    glBufferData(GL_ARRAY_BUFFER ,  3 * 3 * sizeof(float) , mData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);

//    GLuint textures[1];
//    glGenTextures(1 , textures);
//    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textures[0]);
//    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    this->mSurfaceTextureId = textures[0];
}

void VideoApp::renderIdle() {
    glUseProgram(mProgramId);
    glBindBuffer(GL_ARRAY_BUFFER , this->mBufferId);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , 3 * sizeof(float) , 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES , 0 , 3);

    glDisableVertexAttribArray(0);
}

void VideoApp::renderPrepareVideo() {

}

void VideoApp::renderPlayVideo() {
//    if( updateTexImageCompare != updateTexImageCounter ) {
//        // loop and call updateTexImage() for each time the onFrameAvailable() method was called below.
//        while(updateTexImageCompare != updateTexImageCounter) {
//            ASurfaceTexture_updateTexImage(this->mSurfaceTexture);
//            //surfaceTexture.getTransformMatrix(mSTMatrix);
//            updateTexImageCompare++;  // increment the compare value until it's the same as _updateTexImageCounter
//        }
//    }

//    ASurfaceTexture_updateTexImage(this->mSurfaceTexture);
    //LOGI("App Render");

    ASurfaceTexture_getTransformMatrix(this->mSurfaceTexture , this->mUniformSTMat);

    glUseProgram(this->mRenderVideoProgramId);

    glVertexAttribPointer(0, 2 , GL_FLOAT , false , 2 * sizeof(float) , vertexData);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2 , GL_FLOAT , false , 2 * sizeof(float) , textureCoordData);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(this->mUniformSTMatrixLoc , 1 , false , this->mUniformSTMat);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES , this->mSurfaceTextureId);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 4);

    glBindTexture(GL_TEXTURE_2D , 0);
}

void VideoApp::render() {
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (this->mState){
        case IDLE:
            renderIdle();
            break;
        case PREPARE:
            renderPrepareVideo();
            break;
        case PLAYING:
            renderPlayVideo();
            break;
        default:
            break;
    }//end switch
}



void VideoApp::free() {
    glDeleteProgram(mProgramId);

    GLuint bufferIds[1];
    bufferIds[0] = mBufferId;
    glDeleteBuffers(1 , bufferIds);

    GLuint  textureIds[1];
    textureIds[0] = this->mSurfaceTextureId;
    glDeleteTextures(1 , textureIds);
}

void VideoApp::changeState(VideoState state) {
    this->mState = state;
}
