package xyz.panyi.shadedemo;

import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import androidx.appcompat.app.AppCompatActivity;

/**
 *  JAVA层 硬件解码视频
 */
public class PlayVideoActivity extends AppCompatActivity {
    RenderView mRenderView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

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
