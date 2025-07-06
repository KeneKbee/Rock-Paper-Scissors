#include "Shark.h"

Shark::Shark(SDL_Renderer* renderer)
{
	hand = new RoboHand(renderer, handSize);

	blood = AssetLoader::CreateTexture("Vfx/SharkBlood.png", renderer);
	black = AssetLoader::CreateTexture("Vfx/black.png", renderer);

	screen.x = 0;
	screen.y = 0;
	screen.h = 1080;
	screen.w = 1920;

	shortCircutSound = AssetLoader::CreateSound("ShortCircut.wav");
	cutSound = AssetLoader::CreateSound("SharkCut.wav");

	std::cout << "\033[1;33m" << "Created the S.H.A.R.K" << " ^" << "\033[0m" << std::endl;
}

void Shark::updateFingers()
{
	if (lifes == 10 || lifes == 8 || lifes == 6 || lifes == 4 || lifes == 2 || lifes == 0)
	{
		fingers--;
		animationType = R_CUT;
	}
	else {
		Mix_PlayChannel(1, shortCircutSound, 0);
	}
}

void Shark::lose()
{
	lifes--;
	updateFingers();
}

void Shark::reset()
{
	animation_cnt = 0;
	hand->pos.y = R_TOP;
	animationType = R_SHOW;
	currentValue = "";
}

void Shark::resetValue()
{
	currentValue = "";
}

void Shark::show()
{
	animationType = R_SHOW;
}

void Shark::hide()
{
	animationType = R_HIDE;
}

void Shark::check()
{
	animationType = R_CHECK;
}

void Shark::clear()
{
	hand->pos.y = R_TOP;
}

void Shark::hide2()
{
	animationType = R_HIDE_2;
}

void Shark::updateAnimation()
{
	switch (animationType)
	{
		case R_SHOW:
			if (hand->pos.y < R_BOTTOM)
			{
				animation_cnt++;
				hand->show(animation_cnt);
			}
			else {
				animation_cnt = 0;
				animationType = R_WAIT;
			}
			break;

		case R_HIDE:
			if (hand->pos.y > R_TOP)
			{
				hand->hide();
			}
			else
			{
				animationType = R_PICK;
			}
			if (animation_cnt != 0)
			{
				animation_cnt = 0;
			}
			break;

		case R_PICK:
			setRandomHand();
			animationType = R_NONE;
			break;

		case R_WAIT:
			animation_cnt++;
			if (animation_cnt == 100)
			{
				animation_cnt = 0;
				animationType = R_CALL;
			}
			break;

		case R_CHECK:
			break;

		case R_CUT:
			if (animation_cnt == 0)
			{
				Mix_PlayChannel(2, cutSound, 0);
			}

			if (animation_cnt == 200)
			{
				reset();
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

		case R_HIDE_2:
			if (hand->pos.y > R_TOP)
				hand->hide();
			break;

		default:
			break;
	}
}

void Shark::setRandomHand()
{
	srand(time(NULL));
	int sum = playerRockCnt + playerPaperCnt + playerScissorsCnt;
	int r = rand() % sum;

	if (r < playerRockCnt)
	{
		currentValue = "paper"; // counter for rock
	}
	else if (r < playerRockCnt + playerPaperCnt)
	{
		currentValue = "scissors"; // counter for paper
	}
	else {
		currentValue = "rock"; // counter for scissors
	}
}


void Shark::update()
{
	updateAnimation();
}

void Shark::render(SDL_Renderer* renderer)
{
	int handType = 0;

	if (currentValue == "rock")
	{
		handType = 1;
	}
	else if (currentValue == "paper")
	{
		handType = 2;
	}
	else if (currentValue == "scissors")
	{
		handType = 3;
	}

	hand->render(renderer, handType, fingers);

	if(animationType == R_CUT)
	{
		SDL_RenderCopy(renderer, currentFlash, NULL, &screen);
	}
}

int Shark::getAnimation()
{
	return animationType;
}

int Shark::getFingers()
{
	return fingers;
}

std::string Shark::getValue()
{
	return currentValue;
}

bool Shark::isHidden()
{
	if (hand->pos.y <= R_TOP)
	{
		return true;
	}

	return false;
}

Shark::~Shark()
{
	delete hand;
	hand = nullptr;

	SDL_DestroyTexture(blood);
	blood = nullptr;

	SDL_DestroyTexture(black);
	black = nullptr;

	Mix_FreeChunk(shortCircutSound);
	shortCircutSound = nullptr;

	Mix_FreeChunk(cutSound);
	cutSound = nullptr;

	SDL_DestroyTexture(currentFlash);
	currentFlash = nullptr;
}
