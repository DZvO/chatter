#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>
#include <graphics/glew/glew.h>
#include <SDL/SDL.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/projection.hpp"

class Window
{
	public:
		Window();
		~Window();

		void create (const unsigned short width = 800, const unsigned short height = 600, const char* window_title = "WUB WUB WUB", bool smooth_shading = false);
		void resize (const unsigned short width, const unsigned short height);

		void clear();
		void swapBuffers();
		void swap();

		void close ();

		unsigned short getWidth()
		{
			return width;
		}

		unsigned short getHeight()
		{
			return height;
		}

		glm::mat4 * getProjection()
		{
			return &projection;
		}

	private:
		unsigned short width, height;
		SDL_Surface *screen;
		glm::mat4 projection;
};

#endif
