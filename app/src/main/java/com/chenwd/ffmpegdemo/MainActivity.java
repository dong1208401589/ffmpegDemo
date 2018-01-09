package com.chenwd.ffmpegdemo;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
    }

    /*public native void open(String inputPath,String outputPath);

    public void open(View view){
        File input=new File(Environment.getExternalStorageDirectory(),"input.mp4");
        File output=new File(Environment.getExternalStorageDirectory(),"output.yuv");
        open(input.getAbsolutePath(),output.getAbsolutePath());
    }*/
}
