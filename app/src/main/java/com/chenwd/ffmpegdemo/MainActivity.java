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
    static {
        System.loadLibrary("avcodec-56");
        System.loadLibrary("avfilter-5");
        System.loadLibrary("avformat-56");
        System.loadLibrary("avutil-54");
        System.loadLibrary("swresample-1");
        System.loadLibrary("swscale-3");
        System.loadLibrary("native-lib");
    }

    //VideoView videoView;
    private Spinner sp_video;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //videoView= findViewById(R.id.surface);
        sp_video = (Spinner) findViewById(R.id.sp_video);
        //多种格式的视频列表
        String[] videoArray = getResources().getStringArray(R.array.video_list);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1,
                android.R.id.text1, videoArray);
        sp_video.setAdapter(adapter);
    }


    public void mPlay(View view) {
        //String video = sp_video.getSelectedItem().toString();
        //String input = new File(Environment.getExternalStorageDirectory(), video).getAbsolutePath();
        //videoView.player(input);
        playAudio();
    }

    public void stop(View view){
        stopAudio();
    }


    public native void playAudio();

    public native void stopAudio();

    /*public native void open(String inputPath,String outputPath);

    public void open(View view){
        File input=new File(Environment.getExternalStorageDirectory(),"input.mp4");
        File output=new File(Environment.getExternalStorageDirectory(),"output.yuv");
        open(input.getAbsolutePath(),output.getAbsolutePath());
    }*/
}
