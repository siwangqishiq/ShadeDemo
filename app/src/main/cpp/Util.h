

//#ifndef _ASSET_MANAGER_
//#define _ASSET_MANAGER_
//AAssetManager* mAssetManager;
//#endif


#ifndef _UTIL_H
#define _UTIL_H

#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <GLES3/gl3.h>
#include "stb_image.h"

//从Assets中读取文本文件
std::string readAssetTextFile( const char *filename);

std::string jstring2string(JNIEnv *env, jstring jStr);

//从Assets中载入顶点 片段着色器代码
GLuint loadShaderFromAssets(const char *vertexPath , const char *fragPath);

//创建着色器程序
GLuint loadShaderProgram(const char *vShaderSrc, const GLchar *fShaderSrc);

//根据源码创建指定类型的着色器
GLuint createShader(GLenum shaderType, const char *src);

int64_t systemnanotime();

unsigned char* readImage(const char *filename , int &image_width , int &image_height , int &channels);

#endif //SIMPLEPLAYER_LOG_H
