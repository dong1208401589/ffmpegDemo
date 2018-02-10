//
// Created by dc on 2018/1/20.
//

#ifndef FFMPEGDEMO_FFMPEGAUDIO_H
#define FFMPEGDEMO_FFMPEGAUDIO_H


#include <queue>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C"
{
#include <pthread.h>
#include "Log.h"
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>

class FFmpegAudio{
//    函数
public:

    int get(AVPacket *packet);
    int put(AVPacket *packet);

    int play();
    int stop();

    void setAVCodecContext(AVCodecContext *codecContext);

    FFmpegAudio();

    ~FFmpegAudio();

    int createFFmpeg(FFmpegAudio *audio);
    int getPCM(FFmpegAudio *audio);
//成员变量
public:
    int isPlay;
    //索引流
    int index;
//    音频队列
    std::queue<AVPacket *> queue;
//    处理线程
    pthread_t p_playid;
    AVCodecContext *codec;
//同步锁
    pthread_mutex_t mutex;
//    条件变量
    pthread_cond_t cond ;


    SwrContext *swrContext;
    uint8_t *out_buffer;

    //open sl es
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverbItf = NULL;
    SLEnvironmentalReverbSettings settings = SL_I3DL2_ENVIRONMENT_PRESET_DEFAULT;
    SLObjectItf bgPlayerObject = NULL;
    SLPlayItf bqPlayerPlay = NULL;
//队列缓冲区
    SLAndroidSimpleBufferQueueItf bqPlayerQueue = NULL;
//音量对象
    SLVolumeItf bqPalyerVolume = NULL;

    void *buffer;
    size_t bufferSize = 0;
};


};




#endif //FFMPEGDEMO_FFMPEGAUDIO_H
