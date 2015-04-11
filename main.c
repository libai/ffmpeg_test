#include "decode.h"

int video_call(void* p, uint8_t *data, int size, int64_t pts){
	printf("video data size %d", size);
	return 0;
}
int audio_call(void* p, uint8_t *data, int size, int64_t pts){
	printf("audio data size %d", size);
	return 0;
}
void fcb(void* p, int width, int height, int fps){
	printf("width:%d \n", width);
	printf("height:%d \n", height);
	printf("fps:%d \n", fps);
}
int main(){

	const char* filename = "rtsp://127.0.0.1:1235/test1.sdp";
	best_format(NULL, filename, fcb);
	decode_start_best(NULL, filename, video_call, audio_call);

}
