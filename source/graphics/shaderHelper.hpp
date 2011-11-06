#ifndef _SHADER_HELPER_HPP
#define _SHADER_HELPER_HPP

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "lib/glew/glew.h"
#include <GL/gl.h>

class ShaderHelper
{
	public:
	static int loadShader (std::string vertexPath, std::string fragmentPath); //loads the vertex and fragment shaders, and returns the gl_program pointer

	private:
	static char * loadFile (std::string path);
	static bool shader_error (unsigned int object, unsigned int status);
	static void show_error (unsigned int object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
};
#endif
