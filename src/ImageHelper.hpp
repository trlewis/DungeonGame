#include <SDL.h>
#include <SDL_image.h>

#include <iostream>


SDL_Surface* loadSurface(std::string path)
{
	//SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
		std::cout << "Unable to load iamge !" << path.c_str
				<< " SDL_image Error: " << IMG_GetError() << std::endl;

	return loadedSurface;

	//if (loadedSurface == NULL)
	//	//printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	//	std::cout << "Unable to load iamge !" << path.c_str 
	//		<< " SDL_image Error: " << IMG_GetError() << std::endl;
	//else
	//{
	//	optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
	//	if (optimizedSurface == NULL)
	//		//printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

	//	SDL_FreeSurface(loadedSurface);
	//}

	//return optimizedSurface;
}

SDL_Surface* OptimizeSurface(SDL_Surface* surface, SDL_PixelFormat* format)
{
	if (surface == NULL)
		return NULL;

	SDL_Surface* optimized = NULL;
	optimized = SDL_ConvertSurface(surface, format, NULL);
	if (optimized == NULL)
		std::cout << "Unable to optimze image! SDL Error: " << SDL_GetError() << std::endl;

	return optimized;
}