#ifndef _CHATLOG_HPP
#define _CHATLOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

#include <graphics/glew/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphics/window.hpp"
#include "graphics/image.hpp"
#include "graphics/textvertices.hpp"
#include "chat/message.hpp"
#include "helper.hpp"

class Chatlog
{
	public:
	Chatlog(Window * window);
	~Chatlog();

	void add(Message * msg);
	void add(const std::string * str);
	void add(std::string str);

	void draw(Window * window);
	void setLine(const string & input);

	void setWidth (double width)
	{ size.x = width; }
	void setHeight (double height)
	{ size.y = height; }


	private:
	string * line;
	TextVertices * line_vertices;

	glm::vec2 position;
	glm::vec2 size;
	Window * window;
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
