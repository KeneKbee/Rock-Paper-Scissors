#pragma once

#ifndef SHARK_H
#define SHARK_H

#include <cstdlib>
#include <ctime>
#include "RoboHand.h"

// Animation types
#define R_WAIT		0
#define R_HIDE		1
#define R_SHOW		2
#define R_PICK		3
#define R_CHECK		4
#define R_CUT		5
#define R_CALL		6
#define R_HIDE_2	7
#define	R_NONE		8

class Shark // S.H.A.R.K - Strategic Hand Automated Robot Killer
{
public:
	Shark(SDL_Renderer* renderer);
	void lose();
	void reset();
	void resetValue();
	void show();
	void hide();
	void check();
	void clear();
	void hide2();
	void update();
	void render(SDL_Renderer* renderer);
	int getAnimation();
	int getFingers();
	std::string getValue();
	bool isHidden();
	~Shark();
private:
	void updateFingers();
	void updateAnimation();
	void setRandomHand();
public:
	std::string currentValue = "";
	int playerRockCnt = 1;
	int playerPaperCnt = 1;
	int playerScissorsCnt = 1;
private:
	int animation_cnt = 0;
	int animationType = R_SHOW;
	RoboHand* hand = nullptr;
	int handSize = 300;
	int lifes = 12;
	int fingers = 6;
	SDL_Texture* blood = nullptr;
	SDL_Texture* black = nullptr;
	SDL_Texture* currentFlash = nullptr;
	SDL_Rect screen;
	Mix_Chunk* shortCircutSound = nullptr;
	Mix_Chunk* cutSound = nullptr;
};

#endif

