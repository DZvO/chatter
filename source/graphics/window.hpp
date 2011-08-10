#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

const float near_clip = 1.0f, far_clip = 1000.f;

class Window
{
	public:
		Window();
		~Window();

		void create (const unsigned short width = 800, const unsigned short height = 600, const char* window_title = "WUB WUB WUB", bool smooth_shading = true);
		void resize (const unsigned short width, const unsigned short height);

		void clear();
		void swapBuffers();

		void close ();

		unsigned short getWidth();
		unsigned short getHeight();

	private:
		unsigned short width, height;
		SDL_Surface *screen;
};

#endif
