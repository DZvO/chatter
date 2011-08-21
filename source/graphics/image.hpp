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

	private:
	std::string path;
	unsigned int gl_pointer;
	unsigned short width, height, bpp;
};

#endif
