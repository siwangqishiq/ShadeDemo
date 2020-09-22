#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "Log.h"
#include "Util.h"

AAssetManager* mAssetManager;

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
}


extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_render(JNIEnv *env, jclass clazz) {
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_resize(JNIEnv *env, jclass clazz, jint width, jint height) {

}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_free(JNIEnv *env, jclass clazz) {

}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_panyi_shadedemo_NativeBridge_setNativeAssertManager(JNIEnv *env, jclass clazz,
                                                             jobject asset_manager) {
    mAssetManager = AAssetManager_fromJava(env , asset_manager);
    //LOGI("mAssetManager address = %d" , mAssetManager);

    readAssetTextFile("Hello.txt");
}




