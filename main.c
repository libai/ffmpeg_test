#include "decode.h"

void video_call(uint8_t *data, int size, int64_t pts){
	printf("video data size %d", size);
}
void audio_call(uint8_t *data, int size, int64_t pts){
	printf("audio data size %d", size);
}
int main(){

	const char* filename = "rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";

	decode_start(filename, video_call, audio_call);

}