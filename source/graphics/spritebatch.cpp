#include "spritebatch.hpp"
unsigned int SpriteBatch::programPointer = 0;
unsigned int SpriteBatch::positionAttrib = 0;
unsigned int SpriteBatch::texcoordAttrib = 0;
unsigned int SpriteBatch::colorAttrib = 0;
unsigned int SpriteBatch::projectionUniform = 0;
unsigned int SpriteBatch::viewUniform = 0;
unsigned int SpriteBatch::modelUniform = 0;
unsigned int SpriteBatch::texUniform = 0;
unsigned int SpriteBatch::mvpUniform = 0;
std::map<unsigned int, unsigned char*> SpriteBatch::kerning;

SpriteBatch::SpriteBatch ()
{
	programPointer = ShaderHelper::loadShader("data/spritebatch.vert", "data/spritebatch.frag");

	positionAttrib = glGetAttribLocation(programPointer, "position");
	texcoordAttrib = glGetAttribLocation(programPointer, "texcoord");
	colorAttrib = glGetAttribLocation(programPointer, "color");

	//projectionUniform = glGetUniformLocation(programPointer, "projection"); //TODO maybe calculate the mvp on the cpu instead of the gpu so we don't have that much bus traffic? TODO
	//viewUniform = glGetUniformLocation(programPointer, "view");
	//modelUniform = glGetUniformLocation(programPointer, "model");
	mvpUniform = glGetUniformLocation(programPointer, "mvp");
	texUniform = glGetUniformLocation(programPointer, "texture");
	beginCalled = false;
}

void SpriteBatch::begin ()
{
	//TODO set stuff like alpha blending or sort-mode, texture -repeat/wrap, transform-matrix, and so on...
	beginCalled = true;
}

inline void SpriteBatch::addVertex (unsigned int & texId, const short & x, const short & y, const float & texX, const float & texY, unsigned int & color, short depth)
{
	auto * vertexInfo = &vertices[texId];
	vertex_t * vert = &vertexInfo->vertices[vertexInfo->vertexCount];
	vertexInfo->vertexCount++;

	vert->position[0] = x;
	vert->position[1]= y;
	vert->position[2] = depth;
	vert->uv[0] = texX;
	vert->uv[1] = texY;
	vert->color = color;
}

void SpriteBatch::draw (const Image & texture, const Vector2 & position)
{
	draw(texture, Rectangle(position.x, position.y, texture.getWidth(), texture.getHeight()));
}

//Get everything in pixels, later the ortho-projection will have screen values in it so we can use shorts ~~ confused?
void SpriteBatch::draw (const Image & texture, const Rectangle & destination, const Rectangle & uv, const Vector4 & color, const float & rotation, const Vector2 & origin, float scale, const short depth)
{
	unsigned int texid = texture.getId();
	auto it = vertices.find(texid);
	if(it == vertices.end()) //element doesn't exist yet
		vertices[texid].vertexCount = 0; // -> create it!

	Rectangle realuv = uv;
	if(realuv.x == 0 && realuv.y == 0 && realuv.width == 0 && realuv.height == 0)
	{
		realuv.width = texture.getWidth();
		realuv.height = texture.getHeight();
	}

	//convert pixels in uv to 'real' uv coordinate (0 ... 1)
	double uvxmin = (1.0 / texture.getWidth()) * realuv.x;
	double uvxmax = (1.0 / texture.getWidth()) * (realuv.x + realuv.width);
	double uvymin = (1.0 / texture.getHeight()) * realuv.y;
	double uvymax = (1.0 / texture .getHeight()) * (realuv.y + realuv.height);

	//abgr
	unsigned int packed_color = ((unsigned char)(color.a * 255.0f) << 24) | ((unsigned char)(color.b * 255.0f) << 16) | ((unsigned char)(color.g * 255.0f) << 8) | (unsigned char)(color.r * 255.0f);

	/*
	 *			/2
	 *		 / |
	 *    /  |
	 *   0---1
	 */
	//addVertex(texid, destination.x, destination.y + destination.height, uvxmin, uvymax, packed_color); //0
	//addVertex(texid, destination.x + destination.width, destination.y + destination.height, uvxmax, uvymax, packed_color); //1
	//addVertex(texid, destination.x + destination.width, destination.y, uvxmax, uvymin, packed_color); //2

	/*
	 *   1---0
	 *   |  /
	 *   | /
	 *   2/
	 */
	//addVertex(texid, destination.x + destination.width, destination.y, uvxmax, uvymin, packed_color); //0
	//addVertex(texid, destination.x, destination.y, uvxmin, uvymin, packed_color);//1
	//addVertex(texid, destination.x, destination.y + destination.height, uvxmin, uvymax, packed_color);//2

	//save two vertices and hope that it improves performance :)
	/*  0----3
	 *  |    |
	 *  |    |
	 *  1----2
	 */
	if(rotation != 0.0)
	{

		glm::mat3 tm = glm::mat3(1.0);
		tm[0].x = cos(rotation);
		tm[0].y = sin(rotation);
		tm[1].x = -sin(rotation);
		tm[1].y = cos(rotation);

		glm::vec3 v0 = glm::vec3(destination.x, destination.y, 0);
		v0 -= glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);
		v0 = v0 * tm;
		v0 += glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);

		glm::vec3 v1 = glm::vec3(destination.x, destination.y + destination.height, 0);
		v1 -= glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);
		v1 = v1 * tm;
		v1 += glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);

		glm::vec3 v2 = glm::vec3(destination.x + destination.width, destination.y + destination.height, 0);
		v2 -= glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);
		v2 = v2 * tm;
		v2 += glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);

		glm::vec3 v3 = glm::vec3(destination.x + destination.width, destination.y, 0);
		v3 -= glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);
		v3 = v3 * tm;
		v3 += glm::vec3(destination.x + origin.x, destination.y + origin.y, 0);

		addVertex(texid, v0.x, v0.y, uvxmin, uvymin, packed_color, depth);
		addVertex(texid, v1.x, v1.y, uvxmin, uvymax, packed_color, depth);
		addVertex(texid, v2.x, v2.y, uvxmax, uvymax, packed_color, depth);
		addVertex(texid, v3.x, v3.y, uvxmax, uvymin, packed_color, depth);
	}
	else
	{
		addVertex(texid, destination.x, destination.y, uvxmin, uvymin, packed_color, depth);
		addVertex(texid, destination.x, destination.y + destination.height, uvxmin, uvymax, packed_color, depth);
		addVertex(texid, destination.x + destination.width, destination.y + destination.height, uvxmax, uvymax, packed_color, depth);
		addVertex(texid, destination.x + destination.width, destination.y, uvxmax, uvymin, packed_color, depth);
	}
}

void SpriteBatch::end ()
{
	glUseProgram(programPointer);

	//glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getOrthoProjection())));
	//glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
	//glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getOrthoProjection()) * glm::mat4(1.0) * glm::mat4(1.0)));


	for(auto vertices_iterator : vertices)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, vertices_iterator.first);

		//use vertex arrays instead of gl managed vbos since we are using the vertices directly of the ram
		glBindBuffer(GL_ARRAY_BUFFER, 0); //tell gl specificly that we want vertex arrays
		glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_SHORT, GL_FALSE, sizeof(vertex_t), &(vertices_iterator.second.vertices[0].position));
		glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), &(vertices_iterator.second.vertices[0].uv));
		glVertexAttribPointer(colorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex_t), &(vertices_iterator.second.vertices[0].color));

		//glDrawArrays(GL_TRIANGLES, 0, vertices_iterator.second.vertexCount);
		glDrawArrays(GL_QUADS, 0, vertices_iterator.second.vertexCount);

		vertices_iterator.second.vertexCount = 0;
	}

	vertices.clear();
	beginCalled = false;
}

void SpriteBatch::drawString (const Image & font, const std::string & str, const Vector2 & pos, const Vector4 & color, bool useKerning, float scale, const short layer)
{
	if(useKerning)
	{
		if(kerning.find(font.getId()) == kerning.end()) // couldn't find kerning, create it here
		{
			kerning[font.getId()] = new unsigned char[0xff*2]; // left and right kerning for every char in the font-file

			for(int row = 0; row < 16; row++)
			{
				for(int col = 0; col < 16; col++)
				{
					unsigned int left = 0, right = 0;

					//left
					for(int x = col*8; x < col*8+8; x++)
					{
						for(int y = row*8; y < row*8+8; y++)
						{
							if(font.getPixel(x,y) != 0x00)
							{
								left = x;
								//break loops
								x = col*8+8;
								y = row*8+8;
							}
						}
					}

					//unsigned short array_pos = col + (col*row);
					unsigned short array_pos = (row*16) + col;

					for(int x = col*8+8-1; x >= col*8; x--)
					{
						for(int y = row*8+8-1; y >= row*8; y--)
						{
							if(font.getPixel(x, y) != 0x00)
							{
								right = x;
								//break loops
								x = col*8;
								y = row*8;
							}
						}
					}


					kerning[font.getId()][array_pos] = left;
					kerning[font.getId()][array_pos + 0xff] = right;
						//cout << "char (" << col << "|" << row << ") is \"" << (char)array_pos<< "\". left is: " << left << " right is: " << right << '\n';
						//cout << "width is " << right - left << "\n\n";

				}
			}
		}

		int i = 0;
		int x = 0;
		for(unsigned char c : str)
		{
			if(c == ' ' || c == '\0' || c == 0xff)
				x += 3 * scale;
			else
			{
				unsigned int c_row = (unsigned int) c / 16;
				unsigned int c_col = (unsigned int) c % 16;
				//unsigned int array_pos = c_col + (c_col*c_row);

				unsigned int width = kerning[font.getId()][c + 0xff] - kerning[font.getId()][c] + 1;
				//unsigned int width = 8;

				this->draw(font, 
						Rectangle(pos.x + x, pos.y, width*scale, 8*scale),
						Rectangle(kerning[font.getId()][c], c_row*8, width, 8),
						color,
						0.0, Vector2(0.0), 1.0,
						layer
						);

				i++;
				x += (width + 1) * scale;
			}
		}
	}
	else
	{
	}
}

void SpriteBatch::drawDebug()
{
}
