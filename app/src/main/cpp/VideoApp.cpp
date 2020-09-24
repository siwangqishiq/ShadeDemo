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
    glViewport(0 , 0 , viewWidth , viewHeight);
}

void VideoApp::onGetViewSize() {
    this->mProgramId = loadShaderFromAssets("simple_vert.glsl","simple_frag.glsl");

    LOGI("load program id = %d " , this->mProgramId);

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    this->mBufferId = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , mBufferId);
    glBufferData(GL_ARRAY_BUFFER ,  3 * 3 * sizeof(float) , mData , GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void VideoApp::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glUseProgram(mProgramId);
//    glBindBuffer(GL_ARRAY_BUFFER , this->mBufferId);
//    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , 3 * sizeof(float) , 0);
//    glEnableVertexAttribArray(0);
//    glDrawArrays(GL_TRIANGLES , 0 , 3);
//
//    glDisableVertexAttribArray(0);


}

void VideoApp::free() {
    glDeleteProgram(mProgramId);
    GLuint bufferIds[1];
    bufferIds[0] = mBufferId;
    glDeleteBuffers(1 , bufferIds);


    if(mMediaExtractor != nullptr){
        AMediaExtractor_delete(mMediaExtractor);
    }
}

void VideoApp::playVideo(std::string path) {
    this->mState = PREPARE;

    if(this->mMediaExtractor != nullptr){
        AMediaExtractor_delete(this->mMediaExtractor);
    }

    this->mMediaExtractor = AMediaExtractor_new();

    media_status_t status = AMediaExtractor_setDataSource(this->mMediaExtractor , path.c_str());
    LOGI("status = %d" , status);

    this->info.path = path;

    int trackCount = AMediaExtractor_getTrackCount(this->mMediaExtractor);
    LOGI("tracks count =  %d" , trackCount);

    for(int i = 0 ; i < trackCount ; i++){
        AMediaFormat *format =  AMediaExtractor_getTrackFormat(this->mMediaExtractor , i);

        const char* formatStr = AMediaFormat_toString(format);
        const char *mime;
        AMediaFormat_getString(format , AMEDIAFORMAT_KEY_MIME , &mime);
        LOGI("%d track format = %s  mine = %s" ,i ,  formatStr , mime);

        if(strncmp(mime, "video/", 6) == 0){
            int32_t width,height;
            int64_t  duration;

            AMediaFormat_getInt32(format , AMEDIAFORMAT_KEY_WIDTH , &width);
            AMediaFormat_getInt32(format , AMEDIAFORMAT_KEY_HEIGHT , &height);
            AMediaFormat_getInt64(format , AMEDIAFORMAT_KEY_DURATION , &duration);

            LOGI("video info duration = %d , width =  %d , height = %d" ,duration , width , height);

            this->info.duration = duration / 1000;
            this->info.width = width;
            this->info.height = height;

            AMediaExtractor_selectTrack(this->mMediaExtractor , i);

            // set media codec
            if(this->mMediaCodec != nullptr){
                AMediaCodec_stop(this->mMediaCodec);
                AMediaCodec_delete(this->mMediaCodec);
            }

            mMediaCodec = AMediaCodec_createDecoderByType(mime);
            AMediaCodec_configure(this->mMediaCodec , format , nullptr , nullptr , 0);
            addMediaCodecCallback();//添加处理回调
            AMediaCodec_start(this->mMediaCodec);

            break;
        }
    }//end for i
}

void VideoApp::addMediaCodecCallback(){
    mCallback.onAsyncError = onMediaCodecOnAsyncError;
    mCallback.onAsyncFormatChanged = onMediaCodecOnAsyncFormatChanged;
    mCallback.onAsyncInputAvailable = onMediaCodecOnAsyncInputAvailable;
    mCallback.onAsyncOutputAvailable = onMediaCodecOnAsyncOutputAvailable;
    
    AMediaCodec_setAsyncNotifyCallback(this->mMediaCodec , mCallback , this);
}


void onMediaCodecOnAsyncError(AMediaCodec *codec,void *userdata,media_status_t error,int32_t actionCode,const char *detail){
    LOGE("mediacodec error %d  msg :  %s" , actionCode  , detail);
}

void onMediaCodecOnAsyncFormatChanged(AMediaCodec *codec,void *userdata,AMediaFormat *format){

}

void onMediaCodecOnAsyncInputAvailable(AMediaCodec *codec,void *userdata,int32_t index){

}

void onMediaCodecOnAsyncOutputAvailable(AMediaCodec *codec,void *userdata,int32_t index,AMediaCodecBufferInfo *bufferInfo){

}