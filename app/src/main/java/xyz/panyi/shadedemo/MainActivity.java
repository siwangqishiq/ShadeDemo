package xyz.panyi.shadedemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        NativeBridge.setNativeAssertManager(getAssets());

        TextView textView = findViewById(R.id.text);
        textView.setText(NativeBridge.stringFromJNI());
    }
}
