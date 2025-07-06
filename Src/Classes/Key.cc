#include "Key.h"

Key::Key(SDL_Renderer* renderer, std::string value, int x, int y, int size)
{
	
	std::string path = "Keys/" + value + "_key.png";
	keyTexture = AssetLoader::CreateTexture(path, renderer);
	pos.x = x;
	pos.y = y;
	pos.w = size;
	pos.h = size;

	std::cout << "\033[1;33m" << "Created the " << value << " key" << " ^" << "\033[0m" << std::endl;
}

void Key::render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, keyTexture, NULL, &pos);
}

Key::~Key()
{
	SDL_DestroyTexture(keyTexture);
	keyTexture = nullptr;
}
