package xyz.panyi.shadedemo;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.opengl.GLES11Ext;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.Surface;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 *
 */
public class RenderView extends GLSurfaceView implements GLSurfaceView.Renderer , SurfaceTexture.OnFrameAvailableListener  {
    private PlayVideoActivity.Callback mCallback;

    private SurfaceTexture mSurfaceTextture;
    private int mSurfaceTextureId;
    public Surface mSurface;

    public boolean isDecodeRun = false;

    private int _updateTexImageCounter = 0;
    private int _updateTexImageCompare = 0;
    private float mTextureMatrix[] = new float[16];

    public RenderView(Context context) {
        super(context);
        initView();
    }

    public RenderView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    private void initView() {
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setRenderer(this);

        setRenderMode(RENDERMODE_WHEN_DIRTY);
//            setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeBridge.init();

        mSurfaceTextureId = createOesTexture();
        mSurfaceTextture = new SurfaceTexture(mSurfaceTextureId);
        mSurfaceTextture.setOnFrameAvailableListener(this);
        mSurface = new Surface(mSurfaceTextture);

        System.out.println("java SurfaceTextureId = " + mSurfaceTextureId);

        NativeBridge.setSurfaceTexture(mSurfaceTextture , mSurfaceTextureId);
    }

    public void playVideo(String path){
        isDecodeRun = true;
        new DecoderThread(path).start();
        NativeBridge.prepareVideo(mSurfaceTextture);
        requestRender();
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
//            synchronized(this){
        if( _updateTexImageCompare != _updateTexImageCounter ) {
            while(_updateTexImageCompare != _updateTexImageCounter) {
                mSurfaceTextture.updateTexImage();
                mSurfaceTextture.getTransformMatrix(mTextureMatrix);
                NativeBridge.updateTextureMatrix(mTextureMatrix);
                //surfaceTexture.getTransformMatrix(mSTMatrix);
                _updateTexImageCompare++;  // increment the compare value until it's the same as _updateTexImageCounter
            }
        }
//            }


        NativeBridge.render();
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        isDecodeRun = false;
        NativeBridge.free();
    }

    public void setCallback(PlayVideoActivity.Callback mCallback) {
        this.mCallback = mCallback;
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        //surfaceTexture.updateTexImage();
        //System.out.println("onFrameAvailable");
        _updateTexImageCounter++;
        requestRender();
    }

    public Surface getSurface(){
        return mSurface;
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


    class DecoderThread extends Thread {
        final String filepath;

        public DecoderThread(String path){
            filepath = path;
        }

        @Override
        public void run() {

            MediaExtractor mediaExtractor = new MediaExtractor();
            try {
                //final AssetFileDescriptor fileDescriptor = getAssets().openFd("gakki.mp4");
                //FileDescriptor fileDesp  = new FileDescriptor();
                System.out.println("path = " + filepath);

                File file = new File(filepath);
                FileInputStream inputStream = new FileInputStream(file);
                mediaExtractor.setDataSource(inputStream.getFD());

                int numTracks = mediaExtractor.getTrackCount();
                MediaFormat format = null;
                String mineType = null;
                for (int i = 0; i < numTracks; i++) {
                    format = mediaExtractor.getTrackFormat(i);
                    mineType = format.getString(MediaFormat.KEY_MIME);
                    System.out.println("mineType = " + mineType);

                    if (mineType.startsWith("video/")) {
                        // Must select the track we are going to get data by readSampleData()
                        mediaExtractor.selectTrack(i);
                        // Set required key for MediaCodec in decoder mode
                        // Check http://developer.android.com/reference/android/media/MediaFormat.html
                        format.setInteger(MediaFormat.KEY_CAPTURE_RATE, 24);
                        format.setInteger(MediaFormat.KEY_PUSH_BLANK_BUFFERS_ON_STOP, 1);


                        long duration = format.getLong(MediaFormat.KEY_DURATION);
                        int videoWidth = format.getInteger(MediaFormat.KEY_WIDTH);
                        int videoHeight = format.getInteger(MediaFormat.KEY_HEIGHT);
                        NativeBridge.setVideoInfo((int)duration , videoWidth , videoHeight);

                        break;
                    }
                }//end for i

                MediaCodec mediaDecoder = MediaCodec.createDecoderByType(mineType);
                mediaDecoder.configure(format , getSurface() ,null , 0 );
                mediaDecoder.start();

                int timeoutUs = 1_000_000; // 1 second timeout
                boolean eos = false;
                long playStartTime = System.currentTimeMillis();
                long frameDisplayTime = playStartTime;

                MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();

                while (!eos && isDecodeRun) {
                    NativeBridge.playVideo();

                    int inputBufferIndex = mediaDecoder.dequeueInputBuffer(timeoutUs);
                    if (inputBufferIndex >= 0) {
                        ByteBuffer inputBuffer = mediaDecoder.getInputBuffer(inputBufferIndex);
                        int sampleSize = mediaExtractor.readSampleData(inputBuffer, 0);

                        if (sampleSize > 0) {
                            frameDisplayTime = (mediaExtractor.getSampleTime() >> 10) + playStartTime;

                            mediaDecoder.queueInputBuffer(inputBufferIndex, 0, sampleSize, mediaExtractor.getSampleTime(), 0);
                            mediaExtractor.advance();
                        } else {
                            mediaDecoder.queueInputBuffer(inputBufferIndex, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                            eos = true;
                        }
                    }

                    int outputBufferIndex = mediaDecoder.dequeueOutputBuffer(bufferInfo, timeoutUs);

                    if (outputBufferIndex >= 0) {
                        // Frame rate control
                        while (frameDisplayTime > System.currentTimeMillis()) {
                            try {
                                sleep(10);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }//end while

                        // outputBuffer is ready to be processed or rendered.
                        //System.out.println("outputBufferIndex  = " + outputBufferIndex);
                        mediaDecoder.releaseOutputBuffer(outputBufferIndex, true);
                    }
                }//end while

                mediaExtractor.release();

                mediaDecoder.release();
                isDecodeRun = false;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }//end inner class
}
