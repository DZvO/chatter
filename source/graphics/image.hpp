#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>

class Image
{
	public:
	Image();
	Image(std::string path);
	~Image();

	void load(std::string path);

	unsigned int getGlPointer();
	unsigned int getPixel(unsigned int x, unsigned int y);

	private:
	std::string path;
	SDL_Surface * surface;
	unsigned int gl_pointer;
	unsigned short width, height, bpp;
};

#endif
