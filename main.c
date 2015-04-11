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

	const char* filename = "rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
	best_format(NULL, filename, fcb);
	decode_start_best(NULL, filename, video_call, audio_call);

}