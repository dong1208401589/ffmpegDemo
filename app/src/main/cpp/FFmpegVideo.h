//
// Created by dc on 2018/1/20.
//

#ifndef FFMPEGDEMO_FFMPEGVIDEO_H
#define FFMPEGDEMO_FFMPEGVIDEO_H
extern "C"
{
#include <pthread.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include "Log.h"
#include <libavcodec/avcodec.h>
#include <queue>

class FFmpegVideo{
public:
    FFmpegVideo();
    ~FFmpegVideo();

    int get(AVPacket *packet);

    int put(AVPacket *packet);

    void play();

    void stop();

    void setAvCodecContext(AVCodecContext *codecContext);
public:
    //    是否正在播放
    int isPlay;
//    流索引
    int index;
//音频队列
    std::queue<AVPacket *> queue;
//    处理线程
    pthread_t p_playid;
//    解码器上下文
    AVCodecContext *codec;

//    同步锁
    pthread_mutex_t mutex;
//    条件变量
    pthread_cond_t cond;
};

};
#endif //FFMPEGDEMO_FFMPEGVIDEO_H
