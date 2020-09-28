//
// Created by panyi on 2020/9/22.
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

    this->mCamera.resetCamera(0 , 0, viewWidth , viewHeight);
    this->left = 0;
    this->top = viewHeight;
    this->right = viewWidth;
    this->bottom = 0;
    resetVideoVertexData();

    onGetViewSize();
    glViewport(0 , 0 , viewWidth , viewHeight);
}

void VideoApp::onGetViewSize() {
    this->mCamera.resetCamera(0 ,0 , this->viewWidth , this->viewHeight);

    this->mProgramId = loadShaderFromAssets("simple_vert.glsl","simple_frag.glsl");
    LOGI("load mProgramId id = %d " , this->mRenderVideoProgramId);
    this->mRenderVideoProgramId = loadShaderFromAssets("video_vert.glsl","video_frag.glsl");
    LOGI("load mRenderVideoProgramId id = %d " , this->mRenderVideoProgramId);

    this->mUniformSTMatrixLoc = glGetUniformLocation(this->mRenderVideoProgramId , "uSTMatrix");
    this->mUniformEffectTypeLoc = glGetUniformLocation(this->mRenderVideoProgramId , "uEffectType");
    LOGI("shader mUniformEffectTypeLoc  = %d " , this->mUniformEffectTypeLoc);

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

    //ASurfaceTexture_getTransformMatrix(this->mSurfaceTexture , this->mUniformSTMat);

    //resetVideoVertexData();

    glUseProgram(this->mRenderVideoProgramId);

    glVertexAttribPointer(0, 2 , GL_FLOAT , false , 2 * sizeof(float) , vertexData);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2 , GL_FLOAT , false , 2 * sizeof(float) , textureCoordData);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(this->mUniformSTMatrixLoc , 1 , false , this->mUniformSTMat);

    glUniform1i(this->mUniformEffectTypeLoc , 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES , this->mSurfaceTextureId);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 4);

    glBindTexture(GL_TEXTURE_2D , 0);
}

void VideoApp::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    if(this->mState == PLAYING){
        resetLrtpByVideoInfo();
        resetVideoVertexData();
    }
}

void VideoApp::resetLrtpByVideoInfo(){
    LOGI("VideoSize INFO : %d   x  %d  " , this->info.width , this->info.height );

    float ratio = (float)(this->info.width) / (float)(this->info.height);

    if(viewWidth >= viewHeight){
        this->top = viewHeight;
        this->bottom = 0;

        float new_width = ratio * this->viewHeight;
        this->left = viewWidth / 2 - new_width / 2;
        this->right = this->left + new_width;
    }else{//fill width
        this->left = 0;
        this->right = viewWidth;

        float new_height = viewWidth / ratio;
        this->bottom = viewHeight / 2 - new_height / 2;
        this->top = this->bottom + new_height;
    }

    LOGI("left top Right  bottm %f    %f  %f    %f " , this->left , this->top , this->right , this->bottom);
}

/**
 *  重新计算视频坐标值
 *
 */
void VideoApp::resetVideoVertexData(){
    LOGI("viewSize %d   x  %d  " , this->viewWidth , this->viewHeight);
    LOGI("VideoSize %d   x  %d  " , this->info.width , this->info.height );

    this->mCamera.worldToSreen(left , top  ,this->vertexData[0] , this->vertexData[1]);
    this->mCamera.worldToSreen(right , top  ,this->vertexData[2] , this->vertexData[3]);
    this->mCamera.worldToSreen(right , bottom  ,this->vertexData[4] , this->vertexData[5]);
    this->mCamera.worldToSreen(left , bottom  ,this->vertexData[6] , this->vertexData[7]);


    LOGI("vertexData:  %f  %f  %f %f  %f  %f  %f %f " ,
            this->vertexData[0] , this->vertexData[1] ,
            this->vertexData[2] , this->vertexData[3],
            this->vertexData[4] , this->vertexData[5] ,
            this->vertexData[6] , this->vertexData[7] );
}
