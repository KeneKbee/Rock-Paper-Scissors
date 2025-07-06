#include "Hand.h"

Hand::Hand(SDL_Renderer* renderer, std::string value, int handSize)
{
	for (int i = 0; i < 7; ++i) 
	{
		textures[i] = nullptr;
	}

	this->value = value;
	this->handSize = handSize;
	std::string path;
	for(int i = 1; i < 7; i++)
	{
		path = "Player/" + value + "/" + value;
		path += "_" + std::to_string(i-1) + ".png";
		textures[i] = AssetLoader::CreateTexture(path.c_str(), renderer);
	}
	 
	textures[0] = AssetLoader::CreateTexture("Player/hand/hand_null.png", renderer);

	pos.h = handSize;
	pos.w = handSize;

	std::cout << "\033[1;33m" << "Created " << value << " ^" << "\033[0m" << std::endl;
}

void Hand::pick(int animation_cnt)
{
	pos.y = (int) (TOP - std::sin(animation_cnt / 30.0) * 20);
}

void Hand::show(int animation_cnt)
{
	if(pos.y > TOP)
	{
		if (animation_cnt > 100)
		{
			pos.y -= 4;
		}
		else if (animation_cnt > 110)
		{
			pos.y -= 3;
		}
		else if (animation_cnt > 120)
		{
			pos.y -= 2;
		}
		else
		{
			pos.y -= 5;
		}
	}
}

void Hand::hide()
{
	pos.y += 10;
}

void Hand::center()
{
	if (pos.y != TOP)
	{
		if (pos.y > TOP)
		{
			pos.y -= 1;
		}
		else {
			pos.y += 1;
		}
	}
	if (pos.x != CENTER)
	{
		if (pos.x < CENTER)
		{
			pos.x += 5;
		}
		else {
			pos.x -= 5;
		}
	}
}

void Hand::render(SDL_Renderer* renderer, int i)
{
	if(i > 0)
		SDL_RenderCopy(renderer, textures[i], NULL, &pos);
	else
		SDL_RenderCopy(renderer, textures[0], NULL, &pos);
}

Hand::~Hand()
{
	for (int i = 0; i < 7; i++)
	{
		SDL_DestroyTexture(textures[i]);
		textures[i] = nullptr;
	}
}