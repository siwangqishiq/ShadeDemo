//
// Created by panyi on 2020/9/22.
//

#ifndef SHADEDEMO_VIDEOAPP_H
#define SHADEDEMO_VIDEOAPP_H

#include "Log.h"
#include "Util.h"
#include <GLES3/gl3.h>

class VideoApp {
private:
    GLuint mProgramId = -1;
    GLuint mBufferId;

    float mData[6]={
            -1.0  , -1.0 ,
            1.0 , -1.0,
            0.0 , 1.0
                   };
protected:
    int viewWidth;
    int viewHeight;

public:
    void init();

    void onGetViewSize();

    void resize(int w, int h);

    void render();

    void free();
};


#endif //SHADEDEMO_VIDEOAPP_H
