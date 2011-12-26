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
	void upload (bool smooth = true);

	unsigned int getGlPointer();
	unsigned int getPixel(unsigned int x, unsigned int y) const;

	unsigned int * pixels;

	unsigned int getId () const
	{
		return gl_pointer;
	}

	unsigned short getWidth () const
	{
		return width;
	}
	unsigned short getHeight () const
	{
		return height;
	}

	private:
	std::string path;
	SDL_Surface * surface;
	unsigned int gl_pointer;
	unsigned short width, height, bpp;
	static unsigned int texture_count;
};

#endif
