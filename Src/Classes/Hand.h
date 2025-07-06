#pragma once

#ifndef HAND_H
#define HAND_H

#include <iostream>
#include "AssetLoader.h"

// X positions
#define CENTER	(1920/2)-(handSize/2)
#define LEFT	CENTER-handSize-handSize/2
#define RIGHT	CENTER+handSize+handSize/2

// Y positions
#define TOP		(1080/2)+handSize/4
#define BOTTOM	1250

class Hand
{
public:
	Hand(SDL_Renderer* renderer, std::string value, int handSize);
	void pick(int animation_cnt);
	void show(int animation_cnt);
	void hide();
	void center();
	void render(SDL_Renderer* renderer, int i);
	~Hand();
public:
	SDL_Rect pos;
private:
	std::string value = "";
	int handSize = 0;
	SDL_Texture* textures[7];
};

#endif