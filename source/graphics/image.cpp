#include "image.hpp"
unsigned int Image::texture_count(0);

Image::Image()
{
	gl_pointer = width = height = bpp = 0;
	pixels = NULL;
	texture_count++;
}

Image::Image(std::string path)
{
	gl_pointer = width = height = bpp = 0;
	pixels = NULL;
	load(path);
	texture_count++;
}

Image::~Image()
{
	SDL_FreeSurface(surface);
	glDeleteTextures(1, &gl_pointer);
}

void Image::create (unsigned int width, unsigned int height)
{
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
	this->width = width;
	this->height = height;
	this->bpp = surface->format->BitsPerPixel;
	this->pixels = (unsigned int*) surface->pixels;
}

void Image::load(std::string path)
{
	this->path = path;
	surface = IMG_Load(path.c_str());

	if(surface == NULL)
	{
		std::cout << "couldn't load file @ \"" << path << "\"\n";
		return;
	}
	this->width = surface->w;
	this->height = surface->h;
	this->bpp = surface->format->BitsPerPixel;
	this->pixels = (unsigned int*) surface->pixels;
	upload(false);
}

void Image::upload (bool smooth)
{
	unsigned int texture;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (smooth ? GL_LINEAR : GL_NEAREST));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if(surface->format->Amask)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		std::cout << "loaded image " << path << " with amask\n";
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		std::cout << "loaded image " << path << " without amask\n";
	}
	this->gl_pointer = texture;
}

unsigned int Image::getGlPointer()
{
	return gl_pointer;
}

unsigned int Image::getPixel(unsigned int x, unsigned int y)
{
	if(x > width || y > height)
	{
		std::cout << "meep, getPixel was called with too much input! [" << x << ", " << y << "]" << std::endl;
		return -1;
	}

	return ((unsigned int*)surface->pixels) [(y * surface->w) + x];
}
