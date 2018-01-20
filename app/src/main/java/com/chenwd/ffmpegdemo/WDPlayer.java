package com.chenwd.ffmpegdemo;

import android.view.Surface;

/**
 * Created by dc on 2018/1/20.
 */

public class WDPlayer {
    static{
        System.loadLibrary("avcodec-56");
        System.loadLibrary("avdevice-56");
        System.loadLibrary("avfilter-5");
        System.loadLibrary("avformat-56");
        System.loadLibrary("avutil-54");
        System.loadLibrary("postproc-53");
        System.loadLibrary("swresample-1");
        System.loadLibrary("swscale-3");
        System.loadLibrary("native-lib");
    }

    public native void play(String path);

    public native void display(Surface surface);


    public native void  release();
}
