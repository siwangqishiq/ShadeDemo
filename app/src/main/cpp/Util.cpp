//
// Created by panyi on 2020/9/22.
//

#include "Util.h"
#include "Log.h"

extern AAssetManager* mAssetManager;

std::string readAssetTextFile( const char *filename){
    AAsset *file = AAssetManager_open(mAssetManager , filename , AASSET_MODE_BUFFER);
    //LOGI("file read1");
    size_t fileSize = AAsset_getLength(file);

    char *fileContentBuf = new char[fileSize + 1];

    AAsset_read(file , fileContentBuf , fileSize);
    fileContentBuf[fileSize] = '\0';

    AAsset_close(file);

    std::string contentStr = std::string(fileContentBuf);

    delete[] fileContentBuf;

    LOGI("file content = %s" , contentStr.c_str());
    return contentStr;
}

std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, nullptr);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

GLuint loadShaderProgram(const char *vShaderSrc, const GLchar *fShaderSrc){
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vShaderSrc);
    if (!vtxShader)
        return 0;

    fragShader = createShader(GL_FRAGMENT_SHADER, fShaderSrc);
    if (!fragShader)
        return 0;

    program = glCreateProgram();
    if (!program) {
        return 0;
    }

    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

GLuint createShader(GLenum shaderType, const char *src){
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        GLchar* infoLog = (GLchar*)new GLchar[infoLogLen];
        glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
        LOGI("Could not compile %s shader:\n%s\n",
              shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
              infoLog);
        delete[] infoLog;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


GLuint loadShaderFromAssets(const char *vertexPath , const char *fragPath){
    std::string vertexSrc = readAssetTextFile(vertexPath);
    std::string fragSrc = readAssetTextFile(fragPath);

    return loadShaderProgram(vertexSrc.c_str() , fragSrc.c_str());
}

int64_t systemnanotime() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000LL + now.tv_nsec;
}

unsigned char* readImage(const char *filename , int &image_width , int &image_height , int &channels){
    AAsset *file = AAssetManager_open(mAssetManager , filename , AASSET_MODE_BUFFER);
    //LOGI("file read1");
    size_t fileSize = AAsset_getLength(file);
    stbi_uc *fileContentBuf = new stbi_uc[fileSize];
    AAsset_read(file , fileContentBuf , fileSize);
    //stbi_load_from_memory()
    unsigned char *data = stbi_load_from_memory(fileContentBuf , fileSize , &image_width ,&image_height , &channels , 0);

    LOGI("read image file %s  %d %d %d" , filename , image_width , image_height , channels);
    delete[] fileContentBuf;
    AAsset_close(file);
    return data;
}



