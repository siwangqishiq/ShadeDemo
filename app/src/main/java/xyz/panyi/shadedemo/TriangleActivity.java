package xyz.panyi.shadedemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import androidx.appcompat.app.AppCompatActivity;

public class TriangleActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        NativeBridge.setNativeAssertManager(getAssets());

        setContentView(new TriangleView(this));
    }

    private final class TriangleView extends GLSurfaceView
            implements GLSurfaceView.Renderer {
        public TriangleView(Context context) {
            super(context);
            initView();
        }

        private void initView(){
            setEGLContextClientVersion(3);
            setEGLConfigChooser(8, 8, 8, 8, 16, 0);
            setRenderer(this);

            setRenderMode(RENDERMODE_CONTINUOUSLY);
        }

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            TriangleBridge.init();
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int w, int h) {
            TriangleBridge.resize(w , h);
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            TriangleBridge.render();
        }


        @Override
        protected void onDetachedFromWindow() {
            TriangleBridge.free();
            super.onDetachedFromWindow();
        }
    }//end inner class
}
