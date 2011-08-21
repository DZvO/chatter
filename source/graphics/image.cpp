#include "image.hpp"

Image::Image()
{
	gl_pointer = width = height = bpp = 0;
}

Image::Image(std::string path)
{
	load(path);
}

Image::~Image()
{
	glDeleteTextures(1, &gl_pointer);
}

void Image::load(std::string path)
{
	this->path = path;
	SDL_Surface * surface = IMG_Load(path.c_str());

	if(surface == NULL)
	{
		std::cout << "couldn't load file @ \"" << path << "\"\n";
		return;
	}

	unsigned int texture;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if(surface->format->Amask)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}
	this->width = surface->w;
	this->height = surface->h;
	this->bpp = surface->format->BitsPerPixel;
	SDL_FreeSurface(surface);
	this->gl_pointer = texture;
}

unsigned int Image::getGlPointer()
{
	return gl_pointer;
}
