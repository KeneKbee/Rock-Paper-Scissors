#pragma once

#ifndef ROBOHAND_H
#define ROBOHAND_H

#include <iostream>
#include "AssetLoader.h"

// X position
#define R_CENTER	(1920/2)-(handSize/2)

// Y positions
#define R_BOTTOM	(1080/2)-handSize-handSize/4
#define R_TOP		-170-handSize

class RoboHand
{
public:
	RoboHand(SDL_Renderer* renderer, int handSize);
	void show(int animation_cnt);
	void hide();
	void render(SDL_Renderer* renderer, int handType, int i);
	~RoboHand();
public:
	SDL_Rect pos;
private:
	int handSize;
	SDL_Texture* textures[4][7];
};

#endif