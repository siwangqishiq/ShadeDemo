package xyz.panyi.shadedemo;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.SurfaceTexture;

public class NativeBridge {
    static {
        System.loadLibrary("demo");
    }

    public static native String stringFromJNI();

    public static native void setSurfaceTexture(SurfaceTexture sTexture);

    public static native void init();

    public static native void render();

    public static native void resize(int width , int height);

    public static native void free();

    public static native void setNativeAssertManager(AssetManager assetManager);

    public static native void onTouchEvent(int event , float x , float y);

    public static native void playVideoFile(String path);

    public static native void onFrameAvailable();
}
