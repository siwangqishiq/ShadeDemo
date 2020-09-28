//
// Created by Administrator on 2020/9/28.
//

#include "Camera.h"

void Camera::worldToSreen(float input_x , float input_y , float &out_x , float &out_y){
    out_x = 2 *(input_x - x) / view_width - 1.0;
    out_y = 2 * (input_y - y) / view_height - 1.0;
}


