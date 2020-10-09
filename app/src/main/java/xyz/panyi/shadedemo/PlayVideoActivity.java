package xyz.panyi.shadedemo;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.CompoundButton;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import androidx.appcompat.app.AppCompatActivity;

/**
 *  JAVA层 硬件解码视频
 */
public class PlayVideoActivity extends AppCompatActivity {
    RenderView mRenderView;

    RadioGroup mEffectGroup;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        NativeBridge.setNativeAssertManager(getAssets());

        setContentView(R.layout.activity_video);
        mRenderView = findViewById(R.id.render_view);
        mEffectGroup = findViewById(R.id.effect_type_group);

        mRenderView.setCallback((v) -> {
            final String path = getIntent().getStringExtra("path");
            mRenderView.playVideo(path);

            NativeBridge.setVideoEffect(0);
        });

        mEffectGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                int index = group.indexOfChild(findViewById(checkedId));
                System.out.println("checkedId = " + index);
                NativeBridge.setVideoEffect(index);
            }
        });

        if(this.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
            mEffectGroup.setVisibility(View.VISIBLE);
        }else{
            mEffectGroup.setVisibility(View.GONE);
        }
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
