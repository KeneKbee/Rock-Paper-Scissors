#include "Video.h"

Video::Video(SDL_Renderer* renderer, std::string path)
{
	path = "Assets/Videos/" + path;

    if (avformat_open_input(&formatContext, path.c_str(), nullptr, nullptr) != 0)
    {
        throw std::runtime_error("Couldn't open the video file for specified path: (" + path + ")");
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0)
    {
        throw std::runtime_error("Couldn't find the video stream info for specified path: (" + path + ")");
    }

    videoStreamIndex = -1;

    for (unsigned int i = 0; i < formatContext->nb_streams; i++)
    {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1)
        {
            videoStreamIndex = i;
        }
    }

    if (videoStreamIndex == -1)
    {
        throw std::runtime_error("Couldn't find the video stream for specified path: (" + path + ")");
    }


    videoParams = formatContext->streams[videoStreamIndex]->codecpar;
    videoCodec = avcodec_find_decoder(videoParams->codec_id);

    if (videoCodec == nullptr)
    {
        throw std::runtime_error("Couldn't find the codec for the video of specified path: (" + path + ")");
    }

    videoContext = avcodec_alloc_context3(videoCodec);

    if (videoContext == nullptr)
    {
        throw std::runtime_error("Couldn't allocate the codec context of the video for specified path: (" + path + ")");
    }

    if (avcodec_parameters_to_context(videoContext, videoParams) < 0)
    {
        throw std::runtime_error("Couldn't copy the codec params to context of the video for specified path: (" + path + ")");
    }
    
    if (avcodec_open2(videoContext, videoCodec, nullptr) < 0)
    {
        throw std::runtime_error("Couldn't open the codec for the video of specified path: (" + path + ")");
    }


    frame = av_frame_alloc();
    frameYUV = av_frame_alloc();

    if (frame == nullptr || frameYUV == nullptr)
    {
        throw std::runtime_error("Couldn't allocate the frames of the video for specified path: (" + path + ")");
    }

    int width = videoContext->width;
    int height = videoContext->height;

    screen.x = 0;
    screen.y = 0;
    screen.w = 1920;
    screen.h = 1080;

    int numOfBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    buffer = (uint8_t*)av_malloc(numOfBytes * sizeof(uint8_t));

    if (buffer == nullptr)
    {
        throw std::runtime_error("Couldn't allocate the buffer for the video of specified path: (" + path + ")");
    }

    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, buffer, AV_PIX_FMT_YUV420P, screen.w, screen.h, 1);


    swsContext = sws_getContext(width, height, videoContext->pix_fmt,
                           screen.w, screen.h, AV_PIX_FMT_YUV420P,
                           SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (swsContext == nullptr)
    {
        throw std::runtime_error("Couldn't create the sws context for the video of specified path: (" + path + ")");
    }

    currentFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, screen.w, screen.h);

    if (currentFrame == nullptr)
    {
        throw std::runtime_error("Couldn't create a frame of the video for specified path: (" + path + ")");
    }

    std::cout << "\033[1;33m" << "Created a video (" << path << ")" << "\033[0m" << std::endl;
}

void Video::update()
{
    AVPacket packet;
    while (av_read_frame(formatContext, &packet) >= 0)
    {
        if (packet.stream_index == videoStreamIndex)
        {
            if (avcodec_send_packet(videoContext, &packet) == 0)
            {
                if (avcodec_receive_frame(videoContext, frame) == 0)
                {
                    sws_scale(swsContext, frame->data, frame->linesize, 0, videoContext->height,
                        frameYUV->data, frameYUV->linesize);

                    SDL_UpdateYUVTexture(currentFrame, nullptr,
                        frameYUV->data[0], frameYUV->linesize[0],
                        frameYUV->data[1], frameYUV->linesize[1],
                        frameYUV->data[2], frameYUV->linesize[2]);

                    av_packet_unref(&packet);
                    return;
                }
                else {
                    av_packet_unref(&packet);
                }
            }
            else {
                av_packet_unref(&packet);
            }
        }
        else {
            av_packet_unref(&packet);
        }
    }

    isPlaying = false;
}

void Video::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, currentFrame, NULL, &screen);
}

void Video::play()
{
    isPlaying = true;
}

bool Video::playing()
{
    return isPlaying;
}

Video::~Video()
{
    sws_freeContext(swsContext);
    swsContext = nullptr;

    av_free(buffer);
    buffer = nullptr;

    av_frame_free(&frame);
    frame = nullptr;

    av_frame_free(&frameYUV);
    frameYUV = nullptr;

    avcodec_free_context(&videoContext);
    videoContext = nullptr;

    avformat_close_input(&formatContext);
    formatContext = nullptr;

    SDL_DestroyTexture(currentFrame);
    currentFrame = nullptr;
}