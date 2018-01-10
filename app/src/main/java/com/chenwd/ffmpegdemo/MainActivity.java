package com.chenwd.ffmpegdemo;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.

    VideoView videoView;
    private Spinner sp_video;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        videoView= findViewById(R.id.surface);
        sp_video = findViewById(R.id.sp_video);
        //多种格式的视频列表
        String[] videoArray = getResources().getStringArray(R.array.video_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1,
                android.R.id.text1, videoArray);
        sp_video.setAdapter(adapter);
    }


    public void mPlay(View view) {
        String video = sp_video.getSelectedItem().toString();
        String input = new File(Environment.getExternalStorageDirectory(), video).getAbsolutePath();
        videoView.player(input);
    }


    /*public native void open(String inputPath,String outputPath);

    public void open(View view){
        File input=new File(Environment.getExternalStorageDirectory(),"input.mp4");
        File output=new File(Environment.getExternalStorageDirectory(),"output.yuv");
        open(input.getAbsolutePath(),output.getAbsolutePath());
    }*/
}
