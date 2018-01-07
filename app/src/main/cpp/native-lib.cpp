#include <jni.h>
#include <string>
#include "android/log.h"

extern "C"
{
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"
}


#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"wd",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"wd",FORMAT,##__VA_ARGS__);

extern "C"
JNIEXPORT void JNICALL
Java_com_chenwd_ffmpegdemo_MainActivity_open__Ljava_lang_String_2Ljava_lang_String_2(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jstring inputPath_,
                                                                                     jstring outputPath_) {
    const char *inputPath = env->GetStringUTFChars(inputPath_, 0);
    const char *outputPath = env->GetStringUTFChars(outputPath_, 0);

    //注册各大组件
    av_register_all();

    //上下文 分配内存  在转码与播放的时候都会用到这个  包含码流相关的结构体
    AVFormatContext *pContext=avformat_alloc_context();

    //Open an input stream and read the header. The codecs are not opened.
    // The stream must be closed with avformat_close_input().
    if(avformat_open_input(&pContext,inputPath,NULL,NULL)<0){
        LOGE("打开失败");
        return;
    }
//Read packets of a media file to get stream information.
    if (avformat_find_stream_info(pContext,NULL)<0){
        LOGE("获取信息失败");
        return;
    }

    int video_stream_idx=-1;

//    找到视频流
    for (int i = 0; i < pContext->nb_streams; ++i) {
        LOGE("循环 %d" ,i);
//        codec 每一个流对应的解码上下文  codec_type流的类型  AVMEDIA_TYPE_VIDEO 表示视频流
        if (pContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            video_stream_idx=i;
        }
    }
    //获取到解码器上下文
    AVCodecContext * pCodecCtx=pContext->streams[video_stream_idx]->codec;
    //解码器
    AVCodec * pCodex=avcodec_find_decoder(pCodecCtx->codec_id);

    if(avcodec_open2(pCodecCtx,pCodex,NULL)<0){
        LOGE("解码失败");
        return;
    }

    //分配内存  在释放上下文的时候这个内存就会释放   AVPacket  压缩数据
    AVPacket *avPacket= (AVPacket *) av_malloc(sizeof(AVPacket));
//    初始化结构体
    av_init_packet(avPacket);
    AVFrame *frame = av_frame_alloc();

    AVFrame *yuvFrame=av_frame_alloc();
    //给yuvframe 的缓冲区 初始化
    uint8_t  *out_buffer= (uint8_t *) av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));

    avpicture_fill((AVPicture *) yuvFrame, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);


    SwsContext *swsContext=sws_getContext(pCodecCtx->width,pCodecCtx->height,
                                          pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,
                                            AV_PIX_FMT_YUV420P,SWS_BILINEAR,NULL,NULL,NULL);

    FILE *fp_yuv=fopen(outputPath,"wb");

    //packet 入参 出参对象
    int got_frame;
    int frameCount=0;
    while (av_read_frame(pContext,avPacket)>=0){
        //解封装

        //把 avpacket 解压 得到frame
        avcodec_decode_video2(pCodecCtx,frame,&got_frame,avPacket);
        LOGE("解码 %d",frameCount++)
        if (got_frame>0){
            // frame 拿到数据 视频像素数据
            sws_scale(swsContext, (const uint8_t *const *) frame->data, frame->linesize, 0, frame->height,
                      yuvFrame->data, yuvFrame->linesize);

            int y_size=pCodecCtx->width*pCodecCtx->height;
            //写入y亮度信息
            fwrite(yuvFrame->data[0],1,y_size,fp_yuv);
            fwrite(yuvFrame->data[1],1,y_size/4,fp_yuv);
            fwrite(yuvFrame->data[2],1,y_size/4,fp_yuv);
        }
        av_free_packet(avPacket);
    }

    fclose(fp_yuv);
    av_frame_free(&frame);
    av_frame_free(&yuvFrame);
    avcodec_close(pCodecCtx);
    avformat_free_context(pContext);

    env->ReleaseStringUTFChars(inputPath_, inputPath);
    env->ReleaseStringUTFChars(outputPath_, outputPath);
}