//
// Created by dc on 2018/1/20.
//

#include "FFmpegAudio.h"
//消费者
int FFmpegAudio::get(AVPacket *packet) {
    pthread_mutex_lock(&mutex);
    while (isPlay){
        if (!queue.empty()){
            if(av_packet_ref(packet,queue.front())){
                break;
            }
            AVPacket *pkt=queue.front();
            queue.pop();
            av_free(pkt);
        } else{
            //阻塞线程
            pthread_cond_wait(&cond,&mutex);
        }
    }

    return 0;
}
//生产者
int FFmpegAudio::put(AVPacket *packet) {
    AVPacket *packet1= (AVPacket *) av_malloc(sizeof(AVPacket));
    if (av_packet_ref(packet1,packet)){
        LOGE("clone packet fail");
        //克隆失败
        return 0;
    }
//  加锁
    pthread_mutex_lock(&mutex);
//    压入一帧数据
    queue.push(packet1);
    //给消费者解锁
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return 1;
}

void *play_audio(void *arg){
    LOGE("开启音频线程");
    FFmpegAudio *audio= (FFmpegAudio *) arg;
    AVFrame *frame=av_frame_alloc();
    AVPacket *packet= (AVPacket *) av_malloc(sizeof(AVPacket));
    //将mp3等格式转换成pcm
    SwrContext *swrContext=swr_alloc();
    swr_alloc_set_opts(swrContext,
                        AV_CH_LAYOUT_STEREO,
                       AV_SAMPLE_FMT_S16,
                        audio->codec->sample_rate,
                       audio->codec->channel_layout,
                       audio->codec->sample_fmt,
                       audio->codec->sample_rate,
                       0,
                       0
    );

    swr_init(swrContext);
    int out_buffer_size_malloc=44100*2*2;
    uint8_t *out_buffer=(uint8_t *)av_malloc(out_buffer_size_malloc);
//    通道数
    int channels=av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    int got_frame;
    while (audio->isPlay){
        audio->get(packet);
        avcodec_decode_audio4(audio->codec,frame, &got_frame,packet);
        if (got_frame){
            LOGE("解码音频帧");
            swr_convert(swrContext, &out_buffer, out_buffer_size_malloc, (const uint8_t **) frame->data, frame->nb_samples);
            int out_buffer_size=av_samples_get_buffer_size(NULL,channels,frame->nb_samples,AV_SAMPLE_FMT_S16,1);
            //可以播放了得到pcm buffer 的缓存区

        }
    }

}

int FFmpegAudio::play() {
    pthread_create(&p_playid,NULL,play_audio,this);
    return 0;
}

int FFmpegAudio::stop() {
    return 0;
}

void FFmpegAudio::setAVCodecContext(AVCodecContext *codecContext) {
    codec=codecContext;
}

FFmpegAudio::FFmpegAudio() {
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
}

FFmpegAudio::~FFmpegAudio() {

}
