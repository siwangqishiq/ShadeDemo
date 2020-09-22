package xyz.panyi.shadedemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class VideoSurfaceView extends GLSurfaceView implements GLSurfaceView.Renderer  {

    public VideoSurfaceView(Context context) {
        super(context);
        initView();
    }

    public VideoSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    private void initView(){
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setRenderer(this);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeBridge.init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeBridge.resize(width , height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeBridge.render();
    }

    @Override
    protected void onDetachedFromWindow() {
        NativeBridge.free();
        super.onDetachedFromWindow();
    }

}
