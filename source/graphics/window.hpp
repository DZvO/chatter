#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>
#include <SDL/SDL.h>
#include <ctime>
#include "time/stopwatch.hpp"
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
			/**
			 * Returns a/the Window instance, singleton.
			 */
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

			/**
			 * Creates a new Window
			 * @param width The width, in pixels
			 * @param height The height in pixels
			 * @param window_title The window title displayed in the window manager of the user
			 * @param smooth_shading Enable or Disable smooth shading, currently not functional TODO
			 */
			void create (const unsigned short width = 800, const unsigned short height = 600, const char* window_title = "WUB WUB WUB", bool smooth_shading = false);
			/**
			 * Resizes the window to the given dimensions
			 */
			void resize (const unsigned short width, const unsigned short height);

			/**
			 * Clears the window content
			 */
			void clear();
			/**
			 * Swaps the buffers
			 */
			void swap();

			/**
			 * Closes the window, and quits the application
			 */
			void close ();

			/**
			 * Returns the current window width
			 */
			unsigned short getWidth()
			{
				return width;
			}

			/**
			 * Returns the current window height
			 */
			unsigned short getHeight()
			{
				return height;
			}

			/**
			 * Returns a glm::mat4 pointer to the current Orthographic projection matrix
			 */
			glm::mat4 * getOrthoProjection()
			{
				return &ortho_projection;
			}

			/**
			 * Returns a glm::mat4 pointer to the current Perspective projection matrix
			 */
			glm::mat4 * getPerspectiveProjection ()
			{
				return &perspective_projection;
			}

			/**
			 * Enable or disable blocking of Window::swap
			 */
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

			/**
			 * Returns the total running time of the application in milliseconds
			 */
			float getElapsedTime ()
			{
				return SDL_GetTicks();
			}

		private:
			bool block;

			unsigned int getticks, frametimedelta, frametimelast;
			float last;
			float alpha, frametime, framespersecond;
			StopWatch sw;

			unsigned short width, height;
			SDL_Surface *screen;
			glm::mat4 ortho_projection, perspective_projection;
	};
}

#endif
