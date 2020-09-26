package xyz.panyi.shadedemo;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 *
 */
public class VideoSurfaceView extends GLSurfaceView implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    public interface Callback{
        void onSurfaceReady(VideoSurfaceView v);
    }

    public void setCallback(Callback mCallback) {
        this.mCallback = mCallback;
    }

    private Callback mCallback;

    private SurfaceTexture mSurfaceTexture;

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
//        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeBridge.init();
        prepareTexture();
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

        requestRender();
    }

    private void prepareTexture(){
        int textureId = createOesTexture();
        mSurfaceTexture = new SurfaceTexture(textureId);
        mSurfaceTexture.setOnFrameAvailableListener(this);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        NativeBridge.onFrameAvailable();
    }


    /**
     * 创建oes纹理
     * @return
     */
    private int createOesTexture(){
        int[] textures = new int[1];

        GLES30.glGenTextures(1, textures, 0);
        GLES30.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textures[0]);

        GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
        GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);

        GLES30.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
        GLES30.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);

        int textureId = textures[0];
        return textureId;
    }

}//end class
