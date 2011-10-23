#ifndef _CUBE_HPP
#define _CUBE_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glew/glew.h"
#include <GL/gl.h>
#include "graphics/window.hpp"
using motor::Window;

class Cube
{
	class vertex_t
	{
		public:
		glm::vec3 pos;//world position
		glm::vec2 tex;//texture coordinates
		glm::vec3 col;
		vertex_t() {}
		vertex_t(glm::vec3 position, glm::vec2 texcoord) : pos(position), tex(texcoord), col(1.0) {}
		vertex_t(glm::vec3 position, glm::vec2 texcoord, glm::vec3 color) : pos(position), tex(texcoord), col(color) {}
		friend ostream& operator<<(ostream& os, const vertex_t& vt)
		{
			cout << "pos(" << vt.pos.x << "|" << vt.pos.y << "), tex(" << vt.tex.x << "|" << vt.tex.y << ")";
			return os;
		}
	};

	public: 
	Cube ();
	void tick ();
	void draw ();

	private:
	char * loadFile (const char * path);
	glm::vec3 rotation;
	glm::vec3 position;
	glm::mat4 modelmatrix;
	unsigned int programPointer, vertexPointer, fragmentPointer;
	unsigned int positionAttrib, texcoordAttrib, colorAttrib;
	unsigned int projectionUniform, viewUniform, modelUniform, texUniform;

	unsigned int vertexBuffer, vertexCount;
	vertex_t * vertices;
};

#endif
