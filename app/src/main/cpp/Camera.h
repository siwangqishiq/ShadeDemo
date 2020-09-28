//
// Created by Administrator on 2020/9/28.
//

#ifndef SHADEDEMO_CAMERA_H
#define SHADEDEMO_CAMERA_H


class Camera {
public:
    Camera(){
        this->x = 0;
        this->y = 0;
        this->view_width = 2.0;
        this->view_height = 2.0;
    }

    void resetCamera(float _x, float _y, float width, float height){
        this->x = _x;
        this->y = _y;
        this->view_width = width;
        this->view_height = height;
    }

    void worldToSreen(float input_x , float input_y , float &out_x , float &out_y);

    /**
     *  private void reset() {
        //mMatrix.setIdentity();
        mMatrix.m00 = 2 / viewWidth;
        mMatrix.m01 = 0f;
        mMatrix.m02 = (-2*x) / viewWidth - 1;

        mMatrix.m10 = 0f;
        mMatrix.m11 = 2/ viewHeight;
        mMatrix.m12 = (-2 * y) / viewHeight - 1;

        mMatrix.m20 = 0;
        mMatrix.m21 = 0;
        mMatrix.m22 = 1;
    }
     */
private:
    float x = 0;
    float y = 0;
    float view_width;
    float view_height;
};


#endif //SHADEDEMO_CAMERA_H
