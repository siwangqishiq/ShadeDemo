//
// Created by Administrator on 2020/9/22.
//

#include <unistd.h>
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
    this->mRenderVideoProgramId = loadShaderFromAssets("video_vert.glsl","video_frag.glsl");
    LOGI("load program id = %d " , this->mRenderVideoProgramId);

    this->mMVPMatrixLoc = glGetUniformLocation(this->mRenderVideoProgramId , "uMVPMatrix");

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    this->mBufferId = bufferIds[0];

    GLuint textures[1];
    glGenTextures(1 , textures);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textures[0]);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->mSurfaceTextureId = textures[0];

//    glBindBuffer(GL_ARRAY_BUFFER , mBufferId);
//    glBufferData(GL_ARRAY_BUFFER ,  3 * 3 * sizeof(float) , mData , GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ARRAY_BUFFER , 0);

}

void VideoApp::render() {
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glUseProgram(mProgramId);
//    glBindBuffer(GL_ARRAY_BUFFER , this->mBufferId);
//    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , 3 * sizeof(float) , 0);
//    glEnableVertexAttribArray(0);
//    glDrawArrays(GL_TRIANGLES , 0 , 3);
//
//    glDisableVertexAttribArray(0);

    if( updateTexImageCompare != updateTexImageCounter ) {
        // loop and call updateTexImage() for each time the onFrameAvailable() method was called below.
        while(updateTexImageCompare != updateTexImageCounter) {
            ASurfaceTexture_updateTexImage(this->mSurfaceTexture);
            //surfaceTexture.getTransformMatrix(mSTMatrix);
            updateTexImageCompare++;  // increment the compare value until it's the same as _updateTexImageCounter
        }
    }

    ASurfaceTexture_updateTexImage(this->mSurfaceTexture);

    //LOGI("App Render");

    glUseProgram(this->mRenderVideoProgramId);

    glVertexAttribPointer(0, 2 , GL_FLOAT , false , 2 * sizeof(float) , vertexData);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2 , GL_FLOAT , false , 2 * sizeof(float) , textureCoordData);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES , this->mSurfaceTextureId);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 4);

    glBindTexture(GL_TEXTURE_2D , 0);
}

void VideoApp::free() {
    glDeleteProgram(mProgramId);
//    GLuint bufferIds[1];
//    bufferIds[0] = mBufferId;
//    glDeleteBuffers(1 , bufferIds);

    ASurfaceTexture_detachFromGLContext(this->mSurfaceTexture);
    ASurfaceTexture_release(this->mSurfaceTexture);

    GLuint  textureIds[1];
    textureIds[0] = this->mSurfaceTextureId;
    glDeleteTextures(1 , textureIds);

    if(mSurfaceTexture != nullptr){
        delete mSurfaceTexture;
    }

    if(mMediaExtractor != nullptr){
        AMediaExtractor_delete(mMediaExtractor);
    }

    if(mMediaExtractor != nullptr){
        AMediaCodec_stop(mMediaCodec);
        AMediaCodec_delete(mMediaCodec);
    }
}

void VideoApp::onFrameAvailable(){
    LOGI("VideoApp::onFrameAvailable   ！！！%d" , updateTexImageCounter);
    this->updateTexImageCounter++;
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

            ANativeWindow *surface = ASurfaceTexture_acquireANativeWindow(this->mSurfaceTexture);

            AMediaCodec_configure(this->mMediaCodec , format , surface , nullptr , 0);
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

void VideoApp::setSurfaceTexture(JNIEnv *env , jobject s_texture){
    if(mSurfaceTexture != nullptr){
        ASurfaceTexture_release(this->mSurfaceTexture);
    }

    this->mSurfaceTexture = ASurfaceTexture_fromSurfaceTexture(env , s_texture);

    ASurfaceTexture_attachToGLContext(this->mSurfaceTexture , this->mSurfaceTextureId);
}


void onMediaCodecOnAsyncError(AMediaCodec *codec,void *userdata,media_status_t error,int32_t actionCode,const char *detail){
    LOGE("mediacodec error %d  msg :  %s" , actionCode  , detail);
}

void onMediaCodecOnAsyncFormatChanged(AMediaCodec *codec,void *userdata,AMediaFormat *format){
    LOGI("onMediaCodecOnAsyncFormatChanged");
}

void onMediaCodecOnAsyncInputAvailable(AMediaCodec *codec,void *userdata,int32_t index){
    LOGI("onMediaCodecOnAsyncInputAvailable");
    VideoApp *app = (VideoApp *)userdata;

    LOGI("input buffer %zd", index);

    if (index >= 0){
        size_t bufsize;
        uint8_t *buf = AMediaCodec_getInputBuffer(codec , index , &bufsize);
        LOGI("AMediaCodec_getInputBuffer");
        ssize_t sampleSize = AMediaExtractor_readSampleData(app->mMediaExtractor , buf , bufsize);

        LOGI("get SampleSize %zd" , sampleSize);
        if(sampleSize < 0){//文件已经读完了
            sampleSize = 0;
            app->mInputEnd = true;
        }

        int64_t presentationTimeUs = AMediaExtractor_getSampleTime(app->mMediaExtractor);
        LOGI("get AMediaExtractor_getSampleTime %zd" , presentationTimeUs);

        media_status_t status = AMediaCodec_queueInputBuffer(codec , index , 0 , sampleSize , presentationTimeUs,
                app->mInputEnd?AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM:0);
        LOGI("AMediaCodec_queueInputBuffer status = %d" , status);

        bool advance = AMediaExtractor_advance(app->mMediaExtractor);
        LOGI("AMediaExtractor_advance %d" , advance);
    }
}

void onMediaCodecOnAsyncOutputAvailable(AMediaCodec *codec,void *userdata,int32_t index,AMediaCodecBufferInfo *info){
    LOGI("onMediaCodecOnAsyncOutputAvailable");

    VideoApp *app = (VideoApp *)userdata;

    if(index >= 0){
        if(info->flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM){
            LOGI("output AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM");
            app->mOutputEnd = true;
        }

        int64_t presentationNano = info->presentationTimeUs * 1000;
        if(app->renderstart < 0){
            app->renderstart =  systemnanotime() - presentationNano;
        }
        int64_t delay = (app->renderstart + presentationNano) - systemnanotime();
        if(delay > 0){
            usleep(delay / 1000);
        }

        AMediaCodec_releaseOutputBuffer(codec , index , info->size != 0);
    }

}