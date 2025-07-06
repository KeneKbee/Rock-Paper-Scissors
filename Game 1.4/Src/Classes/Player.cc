#include "Player.h"
#include "../Game.h"

Player::Player(SDL_Renderer* renderer)
{
	rock = new Hand(renderer, "rock", handSize);
	paper = new Hand(renderer, "paper", handSize);
	scissors = new Hand(renderer, "scissors", handSize);
	hand = new Hand(renderer, "hand", uiHandSize);

	rock->pos.x = LEFT;
	paper->pos.x = CENTER;
	scissors->pos.x = RIGHT;

	rock->pos.y = BOTTOM;
	paper->pos.y = rock->pos.y;
	scissors->pos.y = rock->pos.y;

	hand->pos.x = (1920 - uiHandSize) - 20;
	hand->pos.y = 20;

	keys[0] = new Key(renderer, "z", LEFT + handSize / 2 - 120 / 2, 1080 - 120, 120);
	keys[1] = new Key(renderer, "x", CENTER + handSize / 2 - 120 / 2 - 7, 1080 - 120, 120);
	keys[2] = new Key(renderer, "c", RIGHT + handSize / 2 - 120 / 2, 1080 - 120, 120);

	blood = AssetLoader::CreateTexture("Vfx/blood.png", renderer);
	black  = AssetLoader::CreateTexture("Vfx/black.png", renderer);

	screen.x = 0;
	screen.y = 0;
	screen.h = 1080;
	screen.w = 1920;

	cutSound = AssetLoader::CreateSound("PlayerCut.wav");

	std::cout << "\033[1;33m" << "Created the player" << " ^" << "\033[0m" << std::endl;
}

void Player::reset()
{
	rock->pos.x = LEFT;
	paper->pos.x = CENTER;
	scissors->pos.x = RIGHT;

	rock->pos.y = BOTTOM;
	paper->pos.y = rock->pos.y;
	scissors->pos.y = rock->pos.y;

	hand->pos.x = (1920 - uiHandSize) - 20;
	hand->pos.y = 20;

	animation_cnt = 0;
	animationType = NONE;
	
	resetValue();
}

void Player::resetValue()
{
	if (currentValue == "rock")
	{
		rockCnt++;
	}
	else if (currentValue == "paper")
	{
		paperCnt++;
	}
	else if (currentValue == "scissors")
	{
		scissorsCnt++;

	}
	currentValue = "";
}

void Player::show()
{
	animationType = SHOW;
}

void Player::hide()
{
	animationType = HIDE;
}

void Player::check()
{
	animationType = CHECK;
}

void Player::clear()
{
	rock->pos.y = BOTTOM;
	paper->pos.y = BOTTOM;
	scissors->pos.y = BOTTOM;

	hideUi();

	animationType = NONE;
}

void Player::move()
{
	if (currentValue != "rock")
	{
		rock->pos.y = -1000;
	}
	if (currentValue != "paper")
	{
		paper->pos.y = -1000;
	}
	if (currentValue != "scissors")
	{
		scissors->pos.y = -1000;
	}
}

void Player::hideUi()
{
	if(hand->pos.y != 300)
	{
		hand->pos.y = -300;
	}
}

void Player::readKey()
{
	switch (Game::event.type)
	{
		case SDL_KEYDOWN:
			switch (Game::event.key.keysym.sym)
			{
				case SDLK_z:
					if (animationType == PICK && lifes > 0)
					{
						currentValue = "rock";
						animationType = HIDE;
					}
					break;

				case SDLK_x:
					if (animationType == PICK && lifes > 0)
					{
						currentValue = "paper";
						animationType = HIDE;
					}
					break;

				case SDLK_c:
					if (animationType == PICK && lifes > 0)
					{
						currentValue = "scissors";
						animationType = HIDE;
					}
					break;
			}
			break;
	}
}

void Player::updateAnimation()
{
	switch (animationType)
	{
		case PICK:
			if (lifes != 0)
			{
				animation_cnt++;
				if(lifes > 0)
				{
					rock->pick(animation_cnt);
					paper->pick(animation_cnt);
					scissors->pick(animation_cnt);
				}
			}
			break;

		case SHOW:
			if (rock->pos.y > TOP || paper->pos.y > TOP)
			{
				animation_cnt++;
				rock->show(animation_cnt);
				paper->show(animation_cnt);
				scissors->show(animation_cnt);
			}
			else
			{
				animationType = PICK;
				animation_cnt = 0;
			}
			break;

		case HIDE:
			if (currentValue != "rock")
			{
				rock->hide();
			}

			if (currentValue != "paper")
			{
				paper->hide();
			}

			if (currentValue != "scissors")
			{
				scissors->hide();
			}

			if ((rock->pos.y > BOTTOM || paper->pos.y > BOTTOM) && (rock->pos.y > 0 && paper->pos.y > 0))
			{
				animationType = SELECT;
			}
			else if((rock->pos.y > BOTTOM || paper->pos.y > BOTTOM || scissors->pos.y > BOTTOM) && (rock->pos.y < 0 || paper->pos.y < 0 || scissors->pos.y < 0))
			{
				animationType = NONE;
			}
			if (animation_cnt != 0)
			{
				animation_cnt = 0;
			}
			break;

		case SELECT:
			if (currentValue == "rock")
			{
				rock->center();
				if (rock->pos.x == CENTER && rock->pos.y == TOP)
				{
					animationType = WAIT;
				}
			}
			else if (currentValue == "paper")
			{
				paper->center();
				if (paper->pos.x == CENTER && paper->pos.y == TOP)
				{
					animationType = WAIT;
				}
			}
			else if (currentValue == "scissors")
			{
				scissors->center();
				if (scissors->pos.x == CENTER && scissors->pos.y == TOP)
				{
					animationType = WAIT;
				}
			}
			break;

		case CUT:
			if (animation_cnt == 0)
			{
				Mix_PlayChannel(0, cutSound, 0);
			}

			if (animation_cnt == 200)
			{
				reset();
				if (lifes == 0)
				{
					hideUi();
				}
			}
			else if (animation_cnt % 10 == 0 && animation_cnt != 0)
			{
				currentFlash = black;
			}
			else if (animation_cnt % 5 == 0)
			{
				currentFlash = blood;
			}
			animation_cnt++;
			break;

		default:
			break;
	}
}

void Player::lose()
{
	lifes--;
	animationType = CUT;
}

void Player::update()
{
	readKey();
	updateAnimation();
}

void Player::render(SDL_Renderer* renderer)
{
	rock->render(renderer, lifes);
	paper->render(renderer, lifes);
	scissors->render(renderer, lifes);
	hand->render(renderer, lifes);

	if (animationType == PICK && lifes != 0)
	{
		for (int i = 0; i < 3; i++)
		{
			keys[i]->render(renderer);
		}
	}

	if(animationType == CUT)
	{
		SDL_RenderCopy(renderer, currentFlash, NULL, &screen);
	}
}

int Player::getAnimation()
{
	return animationType;
}

std::string Player::getValue()
{
	return currentValue;
}

int Player::getLifes()
{
	return lifes;
}

int Player::getRockY()
{
	return rock->pos.y;
}

bool Player::isHidden()
{
	if (rock->pos.y >= BOTTOM || paper->pos.y >= BOTTOM || scissors->pos.y >= BOTTOM)
	{
		return true;
	}

	return false;
}

Player::~Player()
{
	delete rock;
	rock = nullptr;

	delete paper;
	paper = nullptr;

	delete scissors;
	scissors = nullptr;

	delete hand;
	hand = nullptr;

	for (int i = 0; i < 3; i++)
	{
		delete keys[i];
		keys[i] = nullptr;
	}

	SDL_DestroyTexture(blood);
	blood = nullptr;

	SDL_DestroyTexture(black);
	black = nullptr;

	Mix_FreeChunk(cutSound);
	cutSound = nullptr;

	currentFlash = nullptr;
}