#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include "Key.h"

// Animation types
#define PICK		0
#define HIDE		1
#define SHOW		2
#define SELECT		3
#define WAIT		4
#define CHECK		5
#define CUT			6
#define HIDE_2		7
#define NONE		8
#define END			10

class Player
{

public:
	Player(SDL_Renderer* renderer);
	void reset();
	void resetValue();
	void show();
	void hide();
	void check();
	void clear();
	void move();
	void hideUi();
	void lose();
	void update();
	void render(SDL_Renderer* renderer);
	int getAnimation();
	std::string getValue();
	int getLifes();
	int getRockY();
	bool isHidden();
	~Player();
private:
	void readKey();
	void updateAnimation();
public:
	std::string currentValue = "";
	std::string sharkValue = "";
	int rockCnt = 1;
	int paperCnt = 1;
	int scissorsCnt = 1;
private:
	int lifes = 6;
	int handSize = 300;
	int uiHandSize = 150;
	int animationType = NONE;
	int animation_cnt = 0;

	Hand* rock = nullptr;
	Hand* paper = nullptr;
	Hand* scissors = nullptr;
	Hand* hand = nullptr;

	Key* keys[3] = {nullptr, nullptr, nullptr};

	SDL_Texture* blood = nullptr;
	SDL_Texture* black = nullptr;
	SDL_Texture* currentFlash = nullptr;
	SDL_Rect screen;

	Mix_Chunk* cutSound = nullptr;
};

#endif

