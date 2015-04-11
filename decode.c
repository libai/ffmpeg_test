#include "decode.h"
int init = 0;
static int decode_init(){
	avcodec_register_all();
	av_register_all();
	avformat_network_init();
	init = 1;
	return 1;
}

int decode_start_best(void* p, char * filename, video_callback* video_callback(void* p, uint8_t *data, int size, int64_t pts), audio_callback* audio_callback(void* p, uint8_t *data, int size, int64_t pts)){
	if (!init){
		decode_init();
	}
	AVFormatContext *i_fmt_ctx = NULL;
	unsigned video_stream_index = -1;
	unsigned audio_stream_index = -1;


	AVCodecContext* video_dec_ctx = NULL;
	AVCodec* video_dec = NULL;
	AVCodecContext* audio_dec_ctx = NULL;
	AVCodec* audio_dec = NULL;

	
	if (avformat_open_input(&i_fmt_ctx, filename, NULL, NULL) != 0)
	{
		fprintf(stderr, "could not open input file\n");
		return -1;
	}

	if (avformat_find_stream_info(i_fmt_ctx, NULL)<0)
	{
		fprintf(stderr, "could not find stream info\n");
		return -1;
	}

	/* 查找流头部信息 */
	for (unsigned i = 0; i<i_fmt_ctx->nb_streams; i++)
	{
		if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{

			video_stream_index = i;
			//break;
		}
		else if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
			audio_stream_index = i;
		}
	}
	if (video_stream_index != -1){
		video_dec_ctx = i_fmt_ctx->streams[video_stream_index]->codec;
		video_dec = avcodec_find_decoder(video_dec_ctx->codec_id);
		if (avcodec_open2(video_dec_ctx, video_dec, NULL) < 0)
		{
			return 0;
		}
	}
	if (audio_stream_index != -1){
		audio_dec_ctx = i_fmt_ctx->streams[audio_stream_index]->codec;
		audio_dec = avcodec_find_decoder(audio_dec_ctx->codec_id);
		if (avcodec_open2(audio_dec_ctx, audio_dec, NULL) < 0)
		{
			return 0;
		}
	}

	AVPacket i_pkt;
	av_init_packet(&i_pkt);

	while (1)
	{
		//读取一帧数据
		if (av_read_frame(i_fmt_ctx, &i_pkt) < 0){
			fprintf(stderr, "av_read_frame error!\n");
			goto end;
		}
		AVFrame *pFrame = avcodec_alloc_frame();
		int got_picture = 0, ret = 0;

		//判段是否是视频流
		if (i_pkt.stream_index == video_stream_index) {
		
			avcodec_decode_video2(video_dec_ctx, pFrame, &got_picture, &i_pkt);
			if (got_picture) {
				//fprintf(stderr, "decode one video frame!\n");
				if (video_callback(p, i_pkt.data, i_pkt.size, i_pkt.pts) > 0){
					goto end;
				}

			}
			else{
				fprintf(stderr, "decode video frame error!\n");
			}
			//音频流
		}else if (i_pkt.stream_index == audio_stream_index){
				//解码
				avcodec_decode_audio4(audio_dec_ctx, pFrame, &got_picture, &i_pkt);
				if (got_picture) {
					//fprintf(stderr, "decode one audio frame!\n");
					if (audio_callback(p, i_pkt.data, i_pkt.size, i_pkt.pts) > 0){
						goto end;
					}
				}
				else{
					fprintf(stderr, "decode audio frame error!\n");
					goto end;
				}
			
			
		}
		av_free_packet(&i_pkt);
	}

end:
	avformat_close_input(&i_fmt_ctx);

	return 0;



}

int best_format(void* p, const char* filename, format_callback* fcb(void* p, int width, int height, int fps)){
	if (!init){
		decode_init();
	}

	AVFormatContext *i_fmt_ctx = NULL;
	unsigned video_stream_index = -1;
	unsigned audio_stream_index = -1;


	AVCodecContext* video_dec_ctx = NULL;
	AVCodec* video_dec = NULL;
	AVCodecContext* audio_dec_ctx = NULL;
	AVCodec* audio_dec = NULL;


	if (avformat_open_input(&i_fmt_ctx, filename, NULL, NULL) != 0)
	{
		fprintf(stderr, "could not open input file\n");
		return -1;
	}

	if (avformat_find_stream_info(i_fmt_ctx, NULL)<0)
	{
		fprintf(stderr, "could not find stream info\n");
		return -1;
	}

	/* 查找流头部信息 */
	for (unsigned i = 0; i<i_fmt_ctx->nb_streams; i++)
	{
		if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{

			video_stream_index = i;
			//break;
		}
		else if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
			audio_stream_index = i;
		}
	}
	if (video_stream_index != -1){
		video_dec_ctx = i_fmt_ctx->streams[video_stream_index]->codec;
		video_dec = avcodec_find_decoder(video_dec_ctx->codec_id);
		if (avcodec_open2(video_dec_ctx, video_dec, NULL) < 0)
		{
			return 0;
		}

		fcb(p, video_dec_ctx->width, video_dec_ctx->height, 20);
	}
	/*
	if (audio_stream_index != -1){
		audio_dec_ctx = i_fmt_ctx->streams[audio_stream_index]->codec;
		audio_dec = avcodec_find_decoder(audio_dec_ctx->codec_id);
		if (avcodec_open2(audio_dec_ctx, audio_dec, NULL) < 0)
		{
			return 0;
		}
	}
	*/
	
	return 0;
}


