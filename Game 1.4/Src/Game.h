#pragma once

#ifndef GAME_H
#define GAME_H

#include "Classes/AssetLoader.h"
#include "Classes/Player.h"
#include "Classes/Shark.h"
#include "Classes/Video.h"
#include "Classes/MoneyBox.h"

// Parts of game
#define INTRO			0
#define MENU			1
#define GAME			2
#define DEATH			3
#define DEATH_SCREEN	4
#define ENDING			5
#define CREDITS			6

class Game
{
public:
	Game(const char* title, int x, int y, int w, int h, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void playMusic();
	bool running();
	~Game();
private:
	void updateIntro();
	void updateDeath();
	void updateDeathScreen();
	void updateEnding();
	void updateCredits();
	void updateGameAnimations();
	void checkResult();
	void updateHandCounters();
	void updateGame();
	void updateGameType();
	void renderObjects(SDL_Renderer* renderer);
public:
	static SDL_Event event;
private:
	bool isRunning = false;
	int gamePart = INTRO;
	int animation_cnt = 0;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Player* player = nullptr;
	Shark* shark = nullptr;

	MoneyBox* box = nullptr;

	Mix_Music* menuMusic = nullptr;
	Mix_Music* gameMusic = nullptr;
	Mix_Music* deathMusic = nullptr;
	Mix_Music* creditsMusic = nullptr;

	SDL_Texture* menuIMG = nullptr;
	SDL_Texture* backgroundIMG = nullptr;
	SDL_Texture* deathIMG = nullptr;
	SDL_Rect screen;

	Video* intro = nullptr;
	Video* credits = nullptr;

	SDL_Texture* renderTarget = nullptr;

	SDL_Rect innerWindowPos;
};

#endif

