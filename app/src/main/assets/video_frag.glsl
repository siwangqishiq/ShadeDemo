#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;

in vec2 vTextureCoord;

uniform samplerExternalOES sTexture;
out vec4 frag_color;

void main() {
    vec4 originColor = texture(sTexture, vTextureCoord).rgba;
    //frag_color = originColor;
    if(vTextureCoord.x < 0.5){
        frag_color = vec4(originColor.r , 0.0 , 0.0 , 1.0);
    }else{
        frag_color = originColor;
    }
}