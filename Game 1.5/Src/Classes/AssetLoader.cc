#include "AssetLoader.h"

SDL_Texture* AssetLoader::CreateTexture(std::string path, SDL_Renderer* renderer)
{
	path = "Assets/Textures/" + path;

	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	surface = nullptr;

	if (texture != nullptr)
	{
		std::cout << "Created the texture for specified path: (" << path << ")" << std::endl;
	}
	else {
		throw std::runtime_error("Couldn't create the texture for specified path: (" + path + ")");
	}

	return texture;
}

Mix_Music* AssetLoader::CreateMusic(std::string path)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	path = "Assets/Sounds/Music/" + path;

	Mix_Music* music = Mix_LoadMUS(path.c_str());

	Mix_CloseAudio();

	if (music != nullptr)
	{
		std::cout << "Created the music for specified path: (" << path << ")" << std::endl;
	}
	else {
		throw std::runtime_error("Couldn't create the music for specified path: (" + path + ")");
	}

	return music;
}

Mix_Chunk* AssetLoader::CreateSound(std::string path)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	path = "Assets/Sounds/Sfx/" + path;

	Mix_Chunk* sound = Mix_LoadWAV(path.c_str());

	Mix_CloseAudio();

	if (sound != nullptr)
	{
		std::cout << "Created the sound for specified path: (" << path << ")" << std::endl;
	}
	else {
		throw std::runtime_error("Couldn't create the sound for specified path: (" + path + ")");
	}

	return sound;
}
