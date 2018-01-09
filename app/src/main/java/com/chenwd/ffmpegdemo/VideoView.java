package com.chenwd.ffmpegdemo;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by Administrator on 2018/1/6.
 */

public class VideoView extends SurfaceView {

    static {
        System.loadLibrary("avcodec-56");
        System.loadLibrary("avfilter-5");
        System.loadLibrary("avformat-56");
        System.loadLibrary("avutil-54");
        System.loadLibrary("swresample-1");
        System.loadLibrary("swscale-3");
        System.loadLibrary("native-lib");
    }

    public VideoView(Context context) {
        this(context,null);
    }

    public VideoView(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public VideoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    public void init(){
        SurfaceHolder holder=this.getHolder();
        holder.setFormat(PixelFormat.RGBA_8888);
    }

    public void player(final String input){
        new Thread(new Runnable() {
            @Override
            public void run() {
                reader(input,VideoView.this.getHolder().getSurface());
            }
        }).start();
    }

    private native void reader(String input, Surface surface);

}
