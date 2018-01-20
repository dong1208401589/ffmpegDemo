//
// Created by dc on 2018/1/20.
//

#ifndef FFMPEGDEMO_LOG_H
#define FFMPEGDEMO_LOG_H
#include <android/log.h>

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"wd",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"wd",FORMAT,##__VA_ARGS__);

#endif //FFMPEGDEMO_LOG_H
