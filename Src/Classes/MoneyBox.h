#pragma once

#ifndef MONEYBOX_H
#define MONEYBOX_H

#include <iostream>
#include "AssetLoader.h"

class MoneyBox
{
public:
	MoneyBox(SDL_Renderer* renderer);
	void update();
	void render(SDL_Renderer* renderer);
	bool isHidden();
	~MoneyBox();
private:
	SDL_Texture* boxTexture = nullptr;
	SDL_Rect pos;
	SDL_Point center;
	double angle = 0;

	int animation_cnt = 0;
};

#endif
