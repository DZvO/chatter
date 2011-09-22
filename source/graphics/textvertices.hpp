#ifndef _TEXTVERTICES_HPP
#define _TEXTVERTICES_HPP

#include <iostream>
#include <string>
#include <sstream>

#include <cstdio>
#include <cstdlib>
using namespace std;

//#include "graphics/image.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "graphics/image.hpp"
#include "graphics/window.hpp"

class TextVertices
{
	public:
	TextVertices(Window * window, Image * font, glm::vec2 * kern);
	~TextVertices();

	glm::vec2 upload(std::string msg, double scale = 1.0, float r = 1.0, float g = 1.0, float b = 1.0, float width = 1.0, float height = 1.0); //returns width and height used
	glm::vec2 upload(std::string msg, double scale, unsigned char r, unsigned char g, unsigned char b, float width = 1.0, float height = 1.0); //returns width and height used

	void setPosition(glm::vec2 pos);
	const glm::vec2 * getPosition();
	const glm::vec2 * getSize();

	class vertex_t
	{
		public:
		glm::vec3 pos;//world position
		glm::vec2 tex;//texture coordinates
		glm::vec3 col;
		vertex_t() {}
		vertex_t(glm::vec3 position, glm::vec2 texcoord, glm::vec3 color) : pos(position), tex(texcoord), col(color) {}
		friend ostream& operator<<(ostream& os, const vertex_t& vt)
		{
			cout << "pos(" << vt.pos.x << "|" << vt.pos.y << "), tex(" << vt.tex.x << "|" << vt.tex.y << ")";
			return os;
		}
	};


	string str;
	unsigned int getPointer();
	unsigned int getVertexCount();
	private:
	glm::vec2 pos;
	glm::vec2 size;

	unsigned int vertexBuffer;
	vertex_t * vertices;
	unsigned int vertexCount;

	Image * font;
	glm::vec2 * kerning; // x is left to right, y is right to left - empty space in pixels of every 'cell' in the font texture
};

#endif
