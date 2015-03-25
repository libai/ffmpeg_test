#ifndef DECODE_H
#define DECODE_H

#include <libavcodec/avcodec.h> 
#include <libavformat/avformat.h> 

typedef void(*video_callback)(uint8_t *data, int size, int64_t pts);
typedef void(*audio_callback)(uint8_t *data, int size, int64_t pts);
/*
*¿ªÊ¼½âÂë
retrun 1 success 0 fail
*/
int decode_start(char * url, video_callback* video_callback(uint8_t *data, int size, int64_t pts), audio_callback* audio_callback(uint8_t *data, int size, int64_t pts));

//int decode_stop();

#endif