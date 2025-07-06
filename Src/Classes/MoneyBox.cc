#include "MoneyBox.h"

MoneyBox::MoneyBox(SDL_Renderer* renderer)
{
	boxTexture = AssetLoader::CreateTexture("MoneyBox/box.png", renderer);

	pos.w = 600;
	pos.h = 600;
	pos.x = 1920 / 2 - pos.w / 2;
	pos.y = -650;

	center.x = pos.w / 2;
	center.y = pos.h / 2;

	std::cout << "\033[1;33m" << "Created the MoneyBox" << " ^" << "\033[0m" << std::endl;
}

void MoneyBox::update()
{
	if(pos.y < 1080 / 2 - pos.h / 2 || animation_cnt == 150)
	{
		pos.y += 5;

		if(pos.y < 1080 / 2 - pos.h / 2 && pos.y >= 0 - pos.h)
		{
			angle += 0.2;
		}
		else if(pos.y > 1080 / 2 - pos.h / 2 && pos.y <= 1080 + pos.h) {
			angle -= 0.2;
		}
	}
	else {
		animation_cnt++;
	}
}

void MoneyBox::render(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, boxTexture, NULL, &pos, angle, &center, SDL_FLIP_NONE);
}

bool MoneyBox::isHidden()
{
	if (pos.y >= 1080 + 500)
	{
		return true;
	}
	else {
		return false;
	}
}

MoneyBox::~MoneyBox()
{
	SDL_DestroyTexture(boxTexture);
	boxTexture = nullptr;
}
