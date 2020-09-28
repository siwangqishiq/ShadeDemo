#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaFormat.h>
#include "Log.h"
#include "Util.h"
#include "VideoApp.h"


AAssetManager* mAssetManager;
VideoApp *videoApp;

extern "C"
JNIEXPORT jstring JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_stringFromJNI(JNIEnv *env, jclass clazz) {
    LOGI("Hello World!!");
    const char *filename = "Hello.txt";
    std::string hello = readAssetTextFile(filename);
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_init(JNIEnv *env, jclass clazz) {
    // LOGI("surface init");

    videoApp = new VideoApp();
    videoApp->init();
}


extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_render(JNIEnv *env, jclass clazz) {
    //LOGI("surface render");
    videoApp->render();
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_resize(JNIEnv *env, jclass clazz, jint width, jint height) {
    // LOGI("surface resize w x h = %d x %d" , width , height);

    videoApp->resize(width , height);
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_free(JNIEnv *env, jclass clazz) {
    LOGI("surface destoryed");

    videoApp->free();
    delete videoApp;
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_onTouchEvent(JNIEnv *env, jclass clazz, jint event, jfloat x,jfloat y) {
    // LOGI("surface onTouchEvent event %d (%f , %f)" , event , x , y);
    if(videoApp != nullptr){
        videoApp->setTouchPoint(x , y);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_setNativeAssertManager(JNIEnv *env, jclass clazz,
                                                             jobject asset_manager) {
    mAssetManager = AAssetManager_fromJava(env , asset_manager);
    //LOGI("mAssetManager address = %d" , mAssetManager);
    //readAssetTextFile("Hello.txt");
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_playVideoFile(JNIEnv *env, jclass clazz, jstring jpath) {
    std::string path = jstring2string(env ,  jpath);
    LOGI("open file %s" , path.c_str());

//    if(videoApp != nullptr){
//        videoApp->playVideo(path);
//    }
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_onFrameAvailable(JNIEnv *env, jclass clazz) {
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_prepareVideo(JNIEnv *env, jclass clazz) {
    if(videoApp != nullptr){
        //videoApp->setSurfaceTexture(env , s_texture);
        videoApp->changeState(PREPARE);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_playVideo(JNIEnv *env, jclass clazz) {
    if(videoApp != nullptr){
        videoApp->changeState(PLAYING);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_setSurfaceTexture(JNIEnv *env, jclass clazz,
                                                        jobject s_texture, jint s_texture_id) {
    if(videoApp != nullptr){
        videoApp->mSurfaceTextureId = s_texture_id;
        //videoApp->mSurfaceTexture = ASurfaceTexture_fromSurfaceTexture(env , s_texture);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_updateTextureMatrix(JNIEnv *env, jclass clazz, jfloatArray floatArray) {
    jfloat *matData = env->GetFloatArrayElements(floatArray , nullptr);
    if(videoApp != nullptr){
        for(int i = 0 ; i< 16;i++){
            videoApp->mUniformSTMat[i] = matData[i];
        }//end for i
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_setVideoInfo(JNIEnv *env, jclass clazz, jint duration,
                                                   jint video_width, jint video_height) {
    if(videoApp != nullptr){
        videoApp->info.duration = duration;
        videoApp->info.width = video_width;
        videoApp->info.height = video_height;

        videoApp->resetVideoVertexData();
    }
}