package xyz.panyi.shadedemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 *
 */
public class VideoSurfaceView extends GLSurfaceView implements GLSurfaceView.Renderer  {

    public interface Callback{
        void onSurfaceReady(VideoSurfaceView v);
    }

    public void setCallback(Callback mCallback) {
        this.mCallback = mCallback;
    }

    private Callback mCallback;

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

        setRenderMode(RENDERMODE_WHEN_DIRTY);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeBridge.init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeBridge.resize(width , height);

        if(mCallback != null){
            mCallback.onSurfaceReady(this);
        }
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

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        boolean result = false;
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                result = true;
                break;
//            case MotionEvent.ACTION_CANCEL:
//            case MotionEvent.ACTION_UP:
//                result = true;
//                break;
        }

        NativeBridge.onTouchEvent(event.getAction() , event.getX() , event.getY());
        return result;
    }

    public void openFile(String path){
        NativeBridge.playVideoFile(path);
    }
}//end class
