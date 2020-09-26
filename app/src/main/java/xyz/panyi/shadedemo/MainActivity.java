package xyz.panyi.shadedemo;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

import java.util.List;

import me.rosuh.filepicker.config.FilePickerManager;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        NativeBridge.setNativeAssertManager(getAssets());

        TextView textView = findViewById(R.id.text);

        this.requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE} , 100);
        findViewById(R.id.select_file).setOnClickListener((v)->{
            FilePickerManager.INSTANCE.from(this).forResult(FilePickerManager.REQUEST_CODE);
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == FilePickerManager.REQUEST_CODE && resultCode == RESULT_OK) {
            List<String> paths = FilePickerManager.INSTANCE.obtainData();
            if (paths != null && paths.size() > 0) {
                final String filePath = paths.get(0);
                System.out.println("filepath = " + filePath);

                Intent it = new Intent(MainActivity.this , PlayVideoActivity.class);
                it.putExtra("path", filePath);
                startActivity(it);
            }
        }
    }
}
