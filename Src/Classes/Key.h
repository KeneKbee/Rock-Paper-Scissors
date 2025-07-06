#pragma once

#ifndef KEY_H
#define KEY_H

#include <iostream>
#include "AssetLoader.h"

class Key
{
public:
	Key(SDL_Renderer* renderer, std::string value, int x, int y, int size);
	void render(SDL_Renderer* renderer);
	~Key();
public:
	SDL_Rect pos;
private:
	SDL_Texture* keyTexture = nullptr;
};

#endif

