//
// Created by panyi on 2020/9/22.
//

#ifndef SHADEDEMO_VIDEOAPP_H
#define SHADEDEMO_VIDEOAPP_H

#include "Log.h"
#include "Util.h"
#include <GLES3/gl3.h>
#include <string>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaCodec.h>

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

    float mData[3 * 3]={
            -1.0  , -1.0 , 0.0,
            1.0 , -1.0,  0.0,
            0.0 , 1.0 , 0.0};
protected:
    int viewWidth;
    int viewHeight;

    VideoFileInfo info;

    VideoState mState = IDLE;
    AMediaExtractor *mMediaExtractor = nullptr;
    AMediaCodec *mMediaCodec = nullptr;

    AMediaCodecOnAsyncNotifyCallback mCallback;
private:
    void addMediaCodecCallback();

public:
    void init();

    void onGetViewSize();

    void resize(int w, int h);

    void render();

    void free();

    void playVideo(std::string path);

};

void onMediaCodecOnAsyncError(AMediaCodec *codec,void *userdata,media_status_t error,int32_t actionCode,const char *detail);

void onMediaCodecOnAsyncFormatChanged(AMediaCodec *codec,void *userdata,AMediaFormat *format);

void onMediaCodecOnAsyncInputAvailable(AMediaCodec *codec,void *userdata,int32_t index);

void onMediaCodecOnAsyncOutputAvailable(AMediaCodec *codec,void *userdata,int32_t index,AMediaCodecBufferInfo *bufferInfo);

#endif //SHADEDEMO_VIDEOAPP_H
