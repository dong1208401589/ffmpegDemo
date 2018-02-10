//
// Created by dc on 2018/1/20.
//

#include "FFmpegVideo.h"

FFmpegVideo::FFmpegVideo() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

FFmpegVideo::~FFmpegVideo() {

}

int FFmpegVideo::get(AVPacket *packet) {
    pthread_mutex_lock(&mutex);
    while (isPlay) {
        if (!queue.empty()) {
//            从队列取出一个packet   clone一个 给入参对象
            if (av_packet_ref(packet, queue.front())) {
                break;
            }
//            取成功了  弹出队列  销毁packet
            AVPacket *pkt = queue.front();
            queue.pop();
            av_free(pkt);
            break;
        } else {
//            如果队列里面没有数据的话  一直等待阻塞
            pthread_cond_wait(&cond, &mutex);

        }
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

int FFmpegVideo::put(AVPacket *packet) {
    AVPacket *packet1 = (AVPacket *) av_malloc(sizeof(AVPacket));
    if (av_packet_ref(packet1, packet)) {
//        克隆失败
        return 0;
    }
    LOGE("压入一帧数据");
    pthread_mutex_lock(&mutex);
    queue.push(packet1);
//    给消费者解锁
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return 1;
}
void *play_vedio(void *arg){
    FFmpegVideo *vedio = (FFmpegVideo *) arg;
    //像素数据（解码数据）
    AVFrame *frame = av_frame_alloc();

    AVFrame *rgb_frame = av_frame_alloc();
    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_RGBA, vedio->codec->width,
                                                                  vedio->codec->height));
    //设置yuvFrame的缓冲区，像素格式
    avpicture_fill((AVPicture *)rgb_frame, out_buffer
            , AV_PIX_FMT_RGBA, vedio->codec->width, vedio->codec->height);
    //native绘制
    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(vedio->codec->width,
                                                vedio->codec->height,
                                                vedio->codec->pix_fmt,
                                                vedio->codec->width,
                                                vedio->codec->height,
                                                AV_PIX_FMT_RGBA,
                                                SWS_BICUBIC,
                                                NULL,
                                                NULL,
                                                NULL);
    int len ,got_frame, framecount = 0;
    LOGE("宽  %d ,高  %d ",vedio->codec->width,vedio->codec->height);
    //编码数据
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    //6.一阵一阵读取压缩的视频数据AVPacket
    while (vedio->isPlay) {
        LOGE("视频 解码  一帧");
//        消费者取到一帧数据  没有 阻塞
        vedio->get(packet);
        len = avcodec_decode_video2(vedio->codec,frame, &got_frame, packet);
//        转码成rgb
        sws_scale(sws_ctx, (const uint8_t *const *)frame->data, frame->linesize, 0,
                  vedio->codec->height,
                  rgb_frame->data, rgb_frame->linesize);

//        得到了rgb_frame  下节课讲绘制   frame  rgb     pcm  frame




    }
}
void FFmpegVideo::play() {
    isPlay = 1;
    pthread_create(&p_playid, 0, play_vedio, this);
}

void FFmpegVideo::stop() {

}

void FFmpegVideo::setAvCodecContext(AVCodecContext *codecContext) {
    this->codec=codecContext;
}
