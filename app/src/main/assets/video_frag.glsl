#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;

in vec2 vTextureCoord;

uniform int uEffectType;
uniform vec2 uTouchPoint;
uniform vec2 uTextureSize;
uniform int uMosicSize;

uniform samplerExternalOES sTexture;
out vec4 frag_color;

//灰度
vec4 effectGray(vec4 color){
    vec4 result;

    if(gl_FragCoord.x <= uTouchPoint.x){
        float grayVal = color.r*0.299 + color.g*0.587 + color.b*0.114;

        result.r =  grayVal;
        result.g = grayVal;
        result.b = grayVal;
        result.a = 1.0;
    }else{
        result = color;
    }
    return result;
}

//马赛克
vec4 effectMosaic(vec4 color) {
    if(gl_FragCoord.x > uTouchPoint.x){
        return color;
    }

    float s_w = uTextureSize.x;
    float s_h = uTextureSize.y;

    float w_cube = 1.0 / s_w;
    float h_cube  = 1.0/ s_h;

    vec4 result = vec4(0 , 0 , 0, 1.0);

    float mosicWidth = w_cube * float(uMosicSize);
    float mosicHeight = h_cube * float(uMosicSize);

    float x = 0.0;
    float x_offset = 0.0;
    while(x < 1.0){
        x_offset = x + mosicWidth;
        if(x_offset > vTextureCoord.x){
            break;
        }
        x = x_offset;
    }//end while

    float y = 0.0;
    float y_offset = 0.0;
    while(y < 1.0){
        y_offset = y + mosicHeight;
        if(y_offset > vTextureCoord.y){
            break;
        }
        y = y_offset;
    }//end while

    //result = texture(sTexture, vec2(x , y)).rgba;

    int totoal_size = 0;
    for(;x<x_offset ; x+= w_cube){
        for(;y < y_offset ; y+= h_cube){
            vec4 cur_color = texture(sTexture, vec2(x , y)).rgba;

            result.r += cur_color.r;
            result.g += cur_color.g;
            result.b += cur_color.b;

            totoal_size++;
        }
    }
    result.r =  result.r / float(totoal_size);
    result.g = result.g / float(totoal_size);
    result.b = result.b / float(totoal_size);

    return result;
}

void main() {
    vec4 originColor = texture(sTexture, vTextureCoord).rgba;
    switch(uEffectType){
        case 0://正常
        frag_color = originColor;
        break;
        case 1://灰度
        frag_color = effectGray(originColor);
        break;
        case 2://马赛克
        frag_color = effectMosaic(originColor);
        break;
        default:
        frag_color = originColor;
        break;
    }
}