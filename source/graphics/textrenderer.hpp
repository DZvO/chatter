#ifndef _TEXTREDERER_HPP
#define _TEXTREDERER_HPP

#include <iostream>
#include <string>

#include <cstdio>
#include <cstdlib>
using namespace std;

//#include "graphics/image.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "graphics/image.hpp"

struct vertex_t
{
	glm::vec3 pos;//world position
	glm::vec2 tex;//texture coordinates
	vertex_t() {}
	vertex_t(glm::vec3 position, glm::vec2 texcoord) : pos(position), tex(texcoord) {}
};

const unsigned int TEXTURE_CHAR_WIDTH = 8, TEXTURE_CHAR_HEIGHT = 16, TEXTURE_CHAR_GAP = 1;
const unsigned int TEXTURE_WIDTH = 2304;

class TextRenderer
{
	public:
	TextRenderer();
	~TextRenderer();

	void upload(std::string msg, float x, float y, float scale = 1.0);
	void draw();

	private:
	void loadShader();
	void unloadShader();
	//Image * font;

	unsigned int programPointer, fragmentPointer, vertexPointer;
	int positionAttrib, texcoordAttrib;
	unsigned int vertexBuffer;
	vertex_t * vertices;
	unsigned int vertexCount;
	glm::mat4 projection, view, model;
	int projectionUniform, viewUniform, modelUniform, fontTextureUniform;

	Image * font;
};

#endif
