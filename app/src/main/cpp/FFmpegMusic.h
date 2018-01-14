//
// Created by Administrator on 2018/1/14.
//

#ifndef FFMPEGDEMO_FFMPEGMUSIC_H
#define FFMPEGDEMO_FFMPEGMUSIC_H

#endif //FFMPEGDEMO_FFMPEGMUSIC_H

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include <unistd.h>
#include <android/log.h>
}

#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"wd",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"wd",FORMAT,##__VA_ARGS__);

int createFFmpeg(int *rate,int *channel);

void getPCM(void **outBuffer,size_t *size);

void realaseFFmpeg();