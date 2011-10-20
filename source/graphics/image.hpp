#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "lib/glew/glew.h"

class Image
{
	public:
	Image();
	Image(std::string path);
	~Image();

	void create (unsigned int width, unsigned int height);
	void load (std::string path);
	void upload ();

	unsigned int getGlPointer();
	unsigned int getPixel(unsigned int x, unsigned int y);

	unsigned int * pixels;

	private:
	std::string path;
	SDL_Surface * surface;
	unsigned int gl_pointer;
	unsigned short width, height, bpp;
};

#endif
