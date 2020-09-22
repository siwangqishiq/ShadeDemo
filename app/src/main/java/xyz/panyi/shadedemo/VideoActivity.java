package xyz.panyi.shadedemo;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class VideoActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        NativeBridge.setNativeAssertManager(getAssets());

        setContentView(new VideoSurfaceView(this));
    }
}
