#pragma once


#ifndef VIDEO_H
#define VIDEO_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Video
{
public:
	Video(SDL_Renderer* renderer, std::string path);
	void update();
	void render(SDL_Renderer* renderer);
	void play();
	bool playing();
	~Video();
private:
	AVFormatContext* formatContext = nullptr;
	int videoStreamIndex;

	AVCodecParameters* videoParams = nullptr;
	const AVCodec* videoCodec = nullptr;
	AVCodecContext* videoContext = nullptr;

	AVFrame* frame = nullptr;
	AVFrame* frameYUV = nullptr;
	uint8_t* buffer = nullptr;

	SwsContext* swsContext = nullptr;

	SDL_Texture* currentFrame = nullptr;
	SDL_Rect screen;

	bool isPlaying = false;
};

#endif