#include <SDL.h>
#include <SDL_image.h>

#include <iostream>


SDL_Surface* loadSurface2(std::string path)
{
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
		std::cout << "Unable to load iamge !" << path.c_str()
				<< " SDL_image Error: " << IMG_GetError() << std::endl;

	return loadedSurface;
}

SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer)
{
	SDL_Texture* loadedTexture = IMG_LoadTexture(renderer, path.c_str());
	if (loadedTexture == NULL)
		std::cout << "Unable to load iamge !" << path.c_str()
				<< " SDL_image Error: " << IMG_GetError() << std::endl;
	return loadedTexture;
}



SDL_Surface* optimizeSurface(SDL_Surface* surface, SDL_PixelFormat* format)
{
	if (surface == NULL)
		return NULL;

	SDL_Surface* optimized = NULL;
	optimized = SDL_ConvertSurface(surface, format, NULL);
	if (optimized == NULL)
		std::cout << "Unable to optimze image! SDL Error: " << SDL_GetError() << std::endl;

	return optimized;
}

SDL_Surface* loadOptimizedSurface(std::string path, SDL_PixelFormat* format)
{
	SDL_Surface* loaded = loadSurface2(path);
	if (loaded == NULL)
		return loaded;

	return optimizeSurface(loaded, format);
}