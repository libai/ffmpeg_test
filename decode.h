#ifndef DECODE_H
#define DECODE_H

#include <libavcodec/avcodec.h> 
#include <libavformat/avformat.h> 

// return isStopped
typedef int(*video_callback)(void* p, uint8_t *data, int size, int64_t pts);
typedef int(*audio_callback)(void* p, uint8_t *data, int size, int64_t pts);
typedef void(*format_callback)(void* p, int width, int height, int fps);
/*
*start decode
retrun 1 success 0 fail
*/
int decode_start_best(void* p, char * url, video_callback* video_callback(void* p, uint8_t *data, int size, int64_t pts), audio_callback* audio_callback(void* p, uint8_t *data, int size, int64_t pts));

int best_format(void* p, const char* uri, format_callback fcb);

#endif