#include "RoboHand.h"

RoboHand::RoboHand(SDL_Renderer* renderer, int handSize)
{
	for (int i = 0; i < 4; ++i) 
	{
		for (int j = 0; j < 7; ++j) 
		{
			textures[i][j] = nullptr;
		}
	}

	this->handSize = handSize;

	std::string path;
	std::string value;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0:
				value = "hand";
				break;

			case 1:
				value = "rock";
				break;

			case 2:
				value = "paper";
				break;

			case 3:
				value = "scissors";
				break;
		}

		for (int j = 0; j < 7; j++)
		{
			if(j != 0)
			{
				path = "Shark/" + value + "/" + value;
				path += "_" + std::to_string(j - 1) + ".png";
				textures[i][j] = AssetLoader::CreateTexture(path.c_str(), renderer);
			}
			else {
				textures[i][0] = AssetLoader::CreateTexture("Shark/hand/hand_null.png", renderer);
			}
		}
	}

	pos.h = handSize;
	pos.w = handSize;
	pos.x = R_CENTER;
	pos.y = R_TOP;

	std::cout << "\033[1;33m" << "Created RoboHand" << " ^" << "\033[0m" << std::endl;
}

void RoboHand::show(int animation_cnt)
{
	if (pos.y < R_BOTTOM)
	{
		if (animation_cnt > 100)
		{
			pos.y += 4;
		}
		else if (animation_cnt > 110)
		{
			pos.y += 3;
		}
		else if (animation_cnt > 120)
		{
			pos.y += 2;
		}
		else
		{
			pos.y += 5;
		}
	}
}

void RoboHand::hide()
{
	pos.y -= 10;
}

void RoboHand::render(SDL_Renderer* renderer, int handType, int i)
{
	if (i > 0)
		SDL_RenderCopy(renderer, textures[handType][i], NULL, &pos);
	else
		SDL_RenderCopy(renderer, textures[0][0], NULL, &pos);
}

RoboHand::~RoboHand()
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			SDL_DestroyTexture(textures[i][j]);
			textures[i][j] = nullptr;
		}
	}
}