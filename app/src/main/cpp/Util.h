

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

std::string readAssetTextFile( const char *filename);

#endif //SIMPLEPLAYER_LOG_H
