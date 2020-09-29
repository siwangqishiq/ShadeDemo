package xyz.panyi.shadedemo;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.SurfaceTexture;

public class NativeBridge {
    static {
        System.loadLibrary("demo");
    }

    public static final int  VIDEO_EFFECT_NONE = 0;
    public static final int  VIDEO_EFFECT_GRAY = 1;
    public static final int  VIDEO_EFFECT_MOSIAC = 2;
    public static final int  VIDEO_EFFECT_BLUR = 3;

    public static native String stringFromJNI();

    public static native void init();

    public static native void render();

    public static native void prepareVideo(SurfaceTexture sTexture);

    public static native void playVideo();

    public static native void resize(int width , int height);

    public static native void free();

    public static native void setNativeAssertManager(AssetManager assetManager);

    public static native void onTouchEvent(int event , float x , float y);

    public static native void playVideoFile(String path);

    public static native void onFrameAvailable();

    public static native void setSurfaceTexture(SurfaceTexture sTexture , int sTextureId);

    public static native void updateTextureMatrix(float m[]);

    public static native void setVideoInfo(int duration , int videoWidth , int videoHeight);

    public static native void setVideoEffect(int effectType);
}
