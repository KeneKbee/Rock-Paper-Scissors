#pragma once

#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>

class AssetLoader
{
public:
	static SDL_Texture* CreateTexture(std::string path, SDL_Renderer* renderer);
	static Mix_Music* CreateMusic(std::string path);
	static Mix_Chunk* CreateSound(std::string path);
};

#endif