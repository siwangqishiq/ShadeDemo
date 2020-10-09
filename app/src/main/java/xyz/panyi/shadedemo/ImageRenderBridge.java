package xyz.panyi.shadedemo;

public class ImageRenderBridge {
    static {
        System.loadLibrary("demo");
    }

    public static native void init();

    public static native void render();

    public static native void resize(int width , int height);

    public static native void free();
}
