package xyz.panyi.shadedemo;

import android.graphics.SurfaceTexture;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class VideoActivity extends AppCompatActivity {
    private VideoSurfaceView mVideoSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        NativeBridge.setNativeAssertManager(getAssets());
        mVideoSurfaceView = new VideoSurfaceView(this);

        mVideoSurfaceView.setCallback(new VideoSurfaceView.Callback() {
            @Override
            public void onSurfaceReady(VideoSurfaceView v) {
                String path = getIntent().getStringExtra("path");
                mVideoSurfaceView.openFile(path);
            }
        });
        setContentView(mVideoSurfaceView);
    }


}
