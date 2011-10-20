#ifndef _CHATLOG_HPP
#define _CHATLOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

#include "lib/glew/glew.h"
#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"

#include "graphics/window.hpp"
#include "graphics/image.hpp"
#include "graphics/textvertices.hpp"
#include "chat/message.hpp"
#include "helper.hpp"
using namespace motor;

class Chatlog
{
	public:
	Chatlog();
	~Chatlog();

	void add(Message * msg);
	void add(const std::string * str);
	void add(std::string str);

	void draw();
	void setLine(const string & input);

	void setWidth (double width)
	{ size.x = width; }
	void setHeight (double height)
	{ size.y = height; }
	void setSize (double width, double height)
	{
		size.x = width;
		size.y = height;
	}
	void setPosition (double x, double y)
	{
		position.x = x;
		position.y = y;
	}


	string * line;
	private:
	TextVertices * line_vertices;

	glm::vec2 position;
	glm::vec2 size;
	list<Message*> * message_list;
	list<TextVertices*> * vertices_list;

	//void loadShader();
	unsigned int programPointer, fragmentPointer, vertexPointer;
	int positionAttrib, texcoordAttrib, colorAttrib;
	//void unloadShader();

	glm::mat4 view, model;//, projection;
	int projectionUniform, viewUniform, modelUniform, fontTexUniform;

	Image * font;
	glm::vec2 * kerning; // x is left to right, y is right to left - empty space, in pixels, of every 'cell' in the font texture
	void loadKerning();
};

#endif
