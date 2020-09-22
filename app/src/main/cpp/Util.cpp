//
// Created by panyi on 2020/9/22.
//

#include "Util.h"
#include "Log.h"

extern AAssetManager* mAssetManager;

std::string readAssetTextFile( const char *filename){
    AAsset *file = AAssetManager_open(mAssetManager , filename , AASSET_MODE_BUFFER);
    LOGI("file read1");
    size_t fileSize = AAsset_getLength(file);
    LOGI("file read2");
    LOGI("file length = %zu" , fileSize);

    char *fileContentBuf = new char[fileSize + 1];

    AAsset_read(file , fileContentBuf , fileSize);
    fileContentBuf[fileSize] = '\0';

    AAsset_close(file);

    std::string contentStr = std::string(fileContentBuf);

    delete[] fileContentBuf;

    LOGI("file content = %s" , contentStr.c_str());
    return contentStr;
}


