package xyz.panyi.shadedemo;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.opengl.GLES11Ext;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.Surface;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 *  JAVA层 硬件解码视频
 */
public class PlayVideoActivity extends AppCompatActivity {
    RenderView mRenderView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        NativeBridge.setNativeAssertManager(getAssets());

        setContentView(R.layout.activity_video);
        mRenderView = findViewById(R.id.render_view);

        mRenderView.setCallback((v) -> {
            final String path = getIntent().getStringExtra("path");
            mRenderView.playVideo(path);
        });
    }

    public interface Callback {
        void onSurfaceReady(RenderView v);
    }

    /**
     *
     */
//    public static class RenderView extends GLSurfaceView implements GLSurfaceView.Renderer , SurfaceTexture.OnFrameAvailableListener {
//    }//end inner class
}
