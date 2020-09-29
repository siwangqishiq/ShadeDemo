package xyz.panyi.shadedemo;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.opengl.GLES11Ext;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.Surface;

import androidx.annotation.NonNull;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.concurrent.CountDownLatch;

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

    private long renderStart = -1;

    private AudioDecoderTask mAudioTask;

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

//        if(mAudioTask != null){
//            mAudioTask.cancel(true);
//        }
//        mAudioTask = new AudioDecoderTask();
//        mAudioTask.execute(path);

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
    public boolean onTouchEvent(MotionEvent event) {
        boolean result = false;
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                result = true;
                break;
        }

        NativeBridge.onTouchEvent(event.getAction() , event.getX() , event.getY());
        return result;
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        isDecodeRun = false;

        if(mAudioTask != null){
            mAudioTask.cancel(true);
        }
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

    private class DecoderThread extends Thread {
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
                        mediaExtractor.selectTrack(i);
                        // Set required key for MediaCodec in decoder mode
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

                NativeBridge.playVideo();

                boolean eos = false;
                while (!eos && isDecodeRun) {
                    int inputBufferIndex = mediaDecoder.dequeueInputBuffer(2000);

                    long timeoutUs = 0;
                    if (inputBufferIndex >= 0) {
                        ByteBuffer inputBuffer = mediaDecoder.getInputBuffer(inputBufferIndex);
                        int sampleSize = mediaExtractor.readSampleData(inputBuffer, 0);

                        if (sampleSize > 0) {
                            timeoutUs =  mediaExtractor.getSampleTime();
                            mediaDecoder.queueInputBuffer(inputBufferIndex, 0, sampleSize,timeoutUs, 0);
                            mediaExtractor.advance();
                        } else {
                            mediaDecoder.queueInputBuffer(inputBufferIndex, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                            eos = true;
                        }
                    }

                    final MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                    int outputBufferIndex = mediaDecoder.dequeueOutputBuffer(bufferInfo, 0);

                    if (outputBufferIndex >= 0) {
                        // fps控制
                        long presentTimeNano = bufferInfo.presentationTimeUs / 1000;

                        if(renderStart < 0 ){
                            renderStart = System.currentTimeMillis() - presentTimeNano;
                        }
                        long delay = renderStart + presentTimeNano - System.currentTimeMillis();
                        // System.out.println("delayTime  = " + delay);
                        if (delay > 0) {
                            try {
                                Thread.sleep(delay);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }

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

    private class AudioDecoderTask extends AsyncTask<String , Void , Integer> {
        private MediaExtractor extractor;
        private long duration = 0;
        private MediaCodec mediaCodec;

        private CountDownLatch countDownLatch;

        private boolean isAudioEnd = false;
        private int sampleRate;

        private AudioTrack audioTrack;

        @Override
        protected Integer doInBackground(String... paths) {
            extractor = new MediaExtractor();

            countDownLatch = new CountDownLatch(1);
            try {

                String filepath = paths[0];
                File file = new File(filepath);
                FileInputStream inputStream = new FileInputStream(file);
                extractor.setDataSource(inputStream.getFD());

                int numTracks = extractor.getTrackCount();
                MediaFormat format = null;
                String mineType = null;
                for (int i = 0; i < numTracks; i++){
                    format = extractor.getTrackFormat(i);
                    mineType =format.getString(MediaFormat.KEY_MIME);
                    System.out.println("mineType = " + mineType);

                    if(!TextUtils.isEmpty(mineType) && mineType.startsWith("audio")){
                        extractor.selectTrack(i);
                        long value = format.getLong(MediaFormat.KEY_DURATION);
                        duration = value / 1000;
                        System.out.println("时长 = " + duration);

                        sampleRate = format.getInteger(MediaFormat.KEY_SAMPLE_RATE);
                        System.out.println("sampleRate = " + sampleRate);

                        break;
                    }
                }//end for i

                mediaCodec = MediaCodec.createDecoderByType(mineType);

                int buffsize = AudioTrack.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT);
                audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC , sampleRate , AudioFormat.CHANNEL_OUT_STEREO ,
                        AudioFormat.ENCODING_PCM_16BIT , buffsize , AudioTrack.MODE_STREAM);
                audioTrack.play();

                mediaCodec.setCallback(new MediaCodec.Callback(){
                    @Override
                    public void onInputBufferAvailable(@NonNull MediaCodec codec, int index) {
                        if(isAudioEnd)
                            return;

                        final ByteBuffer inputBuf = codec.getInputBuffer(index);
                        int sampleSize = extractor.readSampleData(inputBuf, 0);
                        long timestampTemp = extractor.getSampleTime();

                        System.out.println("input sampleSize = "  + sampleSize);
                        if (sampleSize <= 0){
                            codec.queueInputBuffer(index, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                            isAudioEnd = true;
                            countDownLatch.countDown();
                        }else{
                            codec.queueInputBuffer(index , 0 , sampleSize , timestampTemp , 0);
                        }
                        extractor.advance();
                    }

                    @Override
                    public void onOutputBufferAvailable(@NonNull MediaCodec codec, int index, @NonNull MediaCodec.BufferInfo info) {
                        if(isAudioEnd)
                            return;

                        final ByteBuffer outputBuf =codec.getOutputBuffer(index);
                        System.out.println("outData size = "  + info.size+"   index = " + index);

                        byte[] outData = new byte[info.size];
                        outputBuf.get(outData , 0 , info.size);
                        codec.releaseOutputBuffer(index, true);
                        //audioPlayer.play(outData , 0 , outData.length);
                        audioTrack.write(outData , info.offset , info.offset + info.size);
                    }

                    @Override
                    public void onError(@NonNull MediaCodec codec, @NonNull MediaCodec.CodecException e) {
                        System.out.println("onError ==> " + e.toString());
                    }

                    @Override
                    public void onOutputFormatChanged(@NonNull MediaCodec codec, @NonNull MediaFormat format) {
                        System.out.println("onOutputFormatChanged ==> " + format.getString(MediaFormat.KEY_MIME));
                    }
                });

                mediaCodec.configure(format , null , null , 0);
                mediaCodec.start();

                //mediaCodec.wait(100 * 1000);
                countDownLatch.await();
            } catch (IOException e) {
                e.printStackTrace();
                return -1;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }  finally {
                releaseMedia();
            }
            return 1;
        }

        private void releaseMedia(){
            if(mediaCodec != null){
                mediaCodec.stop();
                mediaCodec.release();
            }

            if(extractor != null){
                extractor.release();
            }

            if(audioTrack != null){
                audioTrack.release();
            }

            System.out.println("end parse file ...");
        }

        @Override
        protected void onCancelled() {
            isAudioEnd = true;
            releaseMedia();
        }
    }//end inner class

}
