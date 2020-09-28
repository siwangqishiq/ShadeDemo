//
// Created by panyi on 2020/9/22.
//

#ifndef SHADEDEMO_VIDEOAPP_H
#define SHADEDEMO_VIDEOAPP_H

#include "Log.h"
#include "Util.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <pthread.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaCodec.h>
#include <android/native_window_jni.h>
#include <android/surface_texture.h>
#include <android/surface_texture_jni.h>
#include "Camera.h"

enum VideoState{
    IDLE,//初始空闲状态
    PREPARE,//准备中
    PLAYING,//播放
    PAUSE,//暂停
    STOP,
};

struct VideoFileInfo{ //视频文件
    std::string path;
    int duration;
    int width;
    int height;
};

class VideoApp {
private:
    GLuint mProgramId = -1;
    GLuint mBufferId;

    GLuint mRenderVideoProgramId;

    float mData[3 * 3]={
            -1.0  , -1.0 , 0.0,
            1.0 , -1.0,  0.0,
            0.0 , 1.0 , 0.0};

    float vertexData[4 * 2] = {
            -1.0f , 1.0f ,
            1.0f , 1.0f ,
            1.0f , -1.0f,
            -1.0f , -1.0f
    };

    float textureCoordData[4 * 2] = {
            0.0f , 1.0f ,
            1.0f , 1.0f ,
            1.0f , 0.0f,
            0.0f , 0.0f
    };

    GLint mUniformSTMatrixLoc;
    GLint mUniformEffectTypeLoc;
    GLint mUfmTouchPointLoc;

    GLint mUfmTextureSizeLoc;
    GLint mUfmMosicSizeLoc;

    float touch_x = -1;
    float touch_y = -1;
public:
    int viewWidth;
    int viewHeight;

    float left , top , right , bottom;

    VideoFileInfo info;

    VideoState mState = IDLE;

    GLuint mSurfaceTextureId;
    ASurfaceTexture *mSurfaceTexture = nullptr;

    Camera mCamera;

    float mUniformSTMat[16];

    void init();

    void onGetViewSize();

    void resize(int w, int h);

    void render();

    void playVideo(std::string path);

    void changeState(VideoState state);

    void renderIdle();

    void renderPlayVideo();

    void renderPrepareVideo();

    void free();

    void setSurfaceTexture(JNIEnv *env , jobject s_texture);

    void onFrameAvailable();

    void resetVideoVertexData();

    void resetLrtpByVideoInfo();

    void setTouchPoint(float _x , float _y);
};

//void onMediaCodecOnAsyncError(AMediaCodec *codec,void *userdata,media_status_t error,int32_t actionCode,const char *detail);
//
//void onMediaCodecOnAsyncFormatChanged(AMediaCodec *codec,void *userdata,AMediaFormat *format);
//
//void onMediaCodecOnAsyncInputAvailable(AMediaCodec *codec,void *userdata,int32_t index);
//
//void onMediaCodecOnAsyncOutputAvailable(AMediaCodec *codec,void *userdata,int32_t index,AMediaCodecBufferInfo *bufferInfo);

#endif //SHADEDEMO_VIDEOAPP_H
