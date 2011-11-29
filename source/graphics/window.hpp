#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>
#include <SDL/SDL.h>
#include <ctime>
using namespace std;

#include "lib/glew/glew.h"
#if defined _WIN32 || WIN32
namespace wgl
{
	#include "lib/glew/wglew.h"
}

#elif defined linux || __GNUC__
namespace glx
{
#include "lib/glew/glxew.h"
}

#else
#error Can not determine which OS you are using
#endif

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtx/projection.hpp"

namespace motor
{
	class Window
	{
		public:
			static Window * getInstance ()
			{
				static Window instance;
				return &instance;
			}
		private:
			Window();
			~Window();
			//prevent copying of the singleton
			Window(Window const&);
			void operator=(Window const &);

		public:

			void create (const unsigned short width = 800, const unsigned short height = 600, const char* window_title = "WUB WUB WUB", bool smooth_shading = false);
			void resize (const unsigned short width, const unsigned short height);

			void clear();
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

			glm::mat4 * getOrthoProjection()
			{
				return &ortho_projection;
			}

			glm::mat4 * getPerspectiveProjection ()
			{
				return &perspective_projection;
			}

			void setBlock (bool b)
			{
				block = b;
			}

			/**
			 * Returns the frametime, in milliseconds
			 */
			float getFrametime ()
			{
				return frametime;
			}

		private:
			bool block;

			unsigned int getticks, frametimedelta, frametimelast;
			float last;
			float alpha, frametime, framespersecond;

			unsigned short width, height;
			SDL_Surface *screen;
			glm::mat4 ortho_projection, perspective_projection;
	};
}

#endif
