//
// Created by Administrator on 2018/1/14.
//

#include "FFmpegMusic.h"

AVFormatContext *pFormatCtx;
int audio_stream_idx=-1;
AVCodecContext *pCodecCtx;
AVCodec *pCodec;
AVFrame *frame;
SwrContext *swrContext;
AVPacket *packet;
uint8_t  *out_buffer;
int out_buffer_size=0;
int createFFmpeg(int *rate,int *channel){
    av_register_all();

    char *input="/sdcard/input.mp3";
    pFormatCtx=avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx,input,NULL,NULL)!=0){
        LOGE("%","打开音频信息失败");
        return -1;
    }
    //3 获取音频信息
    if (avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOGE("%s","获取音频信息失败");
        return -1;
    }

    int i=0;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
            LOGE("找到音频id %d",pFormatCtx->streams[i]->codec->codec_type);
            audio_stream_idx=i;
        }
    }

    //mp3解码器

//    获取音频编解码器
    pCodecCtx=pFormatCtx->streams[audio_stream_idx]->codec;
    LOGE("获取音频编码器上下文 %p",pCodecCtx);
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    LOGE("获取视频编码 %p",pCodec);
    if (avcodec_open2(pCodecCtx,pCodec,NULL)!=0){
        LOGE("Initialize the AVCodecContext to use the given AVCodec on error");
        return -1;
    }
    frame=av_frame_alloc();
    packet= (AVPacket *) av_malloc(sizeof(AVPacket));
    swrContext=swr_alloc();
    int length=0;
    swr_alloc_set_opts(
            swrContext,AV_CH_LAYOUT_STEREO,
            AV_SAMPLE_FMT_S16,
            pCodecCtx->sample_rate,
            pCodecCtx->channel_layout,
            pCodecCtx->sample_fmt,
            pCodecCtx->sample_rate,0,0
    );

    swr_init(swrContext);
    *rate=pCodecCtx->sample_rate;
    *channel=pCodecCtx->channels;
    out_buffer_size=(*rate) * (*channel);
    out_buffer= (uint8_t *) av_malloc(out_buffer_size);
    //LOGE("初始化FFmpeg完毕");

    return 0;

}

void getPCM(void **outBuffer,size_t *size){
    int frameCount=0;
    int got_frame;
    int out_channel_nb=av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    while (av_read_frame(pFormatCtx,packet)>=0){
        //解码 mp3
        avcodec_decode_audio4(pCodecCtx,frame,&got_frame,packet);
        if (got_frame){
            LOGE("解码");
            swr_convert(swrContext,
                        &out_buffer,
                        out_buffer_size,
                        (const uint8_t **) frame->data,
                        frame->nb_samples
            );
         int out_buffer_size=av_samples_get_buffer_size(NULL,
                                                        out_channel_nb,
                                                        frame->nb_samples,
                                                        AV_SAMPLE_FMT_S16,
                                                        1
         );
        *outBuffer=out_buffer;
        *size=out_buffer_size;
            break;
        }
    }
}

void realaseFFmpeg(){
    av_free_packet(packet);
    av_frame_free(&frame);
    av_free(out_buffer);
    swr_free(&swrContext);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
