#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;

in vec2 vTextureCoord;

uniform int uEffectType;

uniform samplerExternalOES sTexture;
out vec4 frag_color;

//灰度
vec4 effectGray(vec4 color){
    vec4 result;
    float grayVal = color.r*0.299 + color.g*0.587 + color.b*0.114;

    result.r =  grayVal;
    result.g = grayVal;
    result.b = grayVal;
    result.a = 1.0;
    return result;
}

void main() {
    vec4 originColor = texture(sTexture, vTextureCoord).rgba;
    //frag_color = originColor;
//    if(vTextureCoord.x < 0.5){
//        frag_color = vec4(originColor.r , 0.0 , 0.0 , 1.0);
//    }else{
//        frag_color = originColor;
//    }
    switch(uEffectType){
        case 0:
        frag_color = originColor;
        break;
        case 1:
        frag_color = effectGray(originColor);
        break;
    }
}