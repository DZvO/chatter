#include "textvertices.hpp"

char * loadFile(const char * path);

TextVertices::TextVertices(Image * font, glm::vec2 * kern)
{
	vertices = NULL;
	this->font = font;
	this->kerning = kern;
	vertexCount = 0;
	vertexBuffer = 0;

	/*glUniform1i(fontTextureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->getGlPointer());*/
}

TextVertices::~TextVertices()
{
	delete [] vertices;
}

glm::vec2 TextVertices::upload(std::string msg, double scale, float r, float g, float b, float width, float height)
{
	using glm::vec3; using glm::vec2;
	vec3 rgb = vec3(r, g, b);
	vec2 rv = vec2(0, 0);

	//vertexCount = msg.length() * 4;
	vertexCount = 0;
	for(unsigned int i = 0; msg[i] != '\0'; i++)
	{
		if(msg[i] == ' ') {	}
		else if(msg[i] == 0xff)
		{
			i += 6;//skip the next 6 (2*3) bytes of color information
		}
		else
		{
			vertexCount++;
		}
	}
	vertexCount *= 4;
	if(vertices != NULL)
	{
		//cout << "deleting vb " << vertexBuffer << endl;
		glDeleteBuffers(1, &vertexBuffer);
		vertexBuffer = 0;
		delete [] vertices;
	}

	{
		glGenBuffers(1, &vertexBuffer);
		//cout << "created vb " << vertexBuffer << endl;
		vertices = new vertex_t[vertexCount];
	}

	const double char_width = double(8) / double(128);
	const double pixel_size = 1.0 / double(128);

	double xl = 0, xr = 0;// + (0.04 - ((kerning[(unsigned char)msg[0]].x * 0.01) + (kerning[(unsigned char)msg[0]].y * 0.01)));
	double yu = 0, yl = 8.0 * 2;
	unsigned int vertex = 0;
	double offset = 0.0;
	for(unsigned int i = 0; msg[i] != '\0'; i++)
	{
		 // Viewport:
		 //     0 y
		 //       |
		 // 0 x -o- +1 x ...
		 //    	  |
		 //     +1 y     
		 //     ...
		unsigned char c = msg[i];

		if(c == ' ')//add check for color setting stuff
		{
			xl = xr;
			xr += 6.0;//(0.03 * scale);
		}
		else if(c == '\n')// || (xr + (0.08 - ((kerning[c].x * 0.01) + (kerning[c].y * 0.01)) + 0.01)) > 1)
		{
			yu = yl + 1.0;
			yl += (8.0 + 1.0) * 2;
			xl = 0;
			rv.x = xr;
			xr = (8.0 - (kerning[c].x + kerning[c].y + 0)) * 2;
			offset = 0.0;
		}
		else if(c == 0xff)
		{
			std::stringstream ssr;
			unsigned int r_byte;
			ssr << std::hex << string(1, msg[i+1]) + string(1, msg[i+2]);
			ssr >> r_byte;

			std::stringstream ssg;
			unsigned int g_byte;
			ssg << std::hex << string(1, msg[i+3]) + string(1, msg[i+4]);
			ssg >> g_byte;

			std::stringstream ssb;
			unsigned int b_byte;
			ssb << std::hex << string(1, msg[i+5]) + string(1, msg[i+6]);
			ssb >> b_byte;

			//cout << "r " << r_byte << endl;
			//cout << "g " << g_byte << endl;
			//cout << "b " << b_byte << endl;

			rgb.r = double(r_byte) / double(0xff);
			rgb.g = double(g_byte) / double(0xff);
			rgb.b = double(b_byte) / double(0xff);
			i += 6;
		}
		else
		{
			unsigned int c_row = (unsigned int)c / 16;
			unsigned int c_col = (unsigned int)c % 16;

			//line break? TODO since we're now using transformation matrices to move the text we can't use screen dimensions on uploading anymore. FIX -> use glm::vec2 size
			if((xr + (8.0 - (kerning[c].x + kerning[c].y + 0)) * 2) > width)
			{
				yu = yl + 1.0;
				yl += (8.0 + 1.0) * 2;

				rv.x = xr;
				xl = 0;
				xr = (8.0 - (kerning[c].x + kerning[c].y + 0)) * 2;
			}
			else
			{
				//do kerning stuff
				xl = xr + 2.0;
				//xr += ((0.4 - (((kerning[c].x * 0.05) + (kerning[c].y * 0.05)))) * scale) + (offset * scale);
				//xr += 8.0 * 2;
				xr += (8.0 - (kerning[c].x + kerning[c].y + 0)) * 2 + 2.0;
				//offset = 0.005000000000;
			}

			double texxleft = c_col * char_width;
			texxleft += (kerning[c].x * pixel_size);
			double texxright = c_col * char_width + char_width;
			texxright -= (kerning[c].y * pixel_size);

			double texyupper = c_row * char_width;
			double texylower = c_row * char_width + char_width;

			//cout << "char " << c << endl;
			vertex_t lowerleft = vertex_t(vec3(xl, yl, 0),
					vec2(texxleft, texylower),
					vec3(rgb.r, rgb.g, rgb.b));
			//cout << lowerleft << endl;

			vertex_t lowerright = vertex_t(vec3(xr, yl, 0),
					vec2(texxright, texylower),
					vec3(rgb.r, rgb.g, rgb.b));
			//cout << lowerright << endl;

			vertex_t upperright = vertex_t(vec3(xr, yu, 0),
					vec2(texxright, texyupper),
					vec3(rgb.r, rgb.g, rgb.b));
			//cout << upperright << endl;

			vertex_t upperleft = vertex_t(vec3(xl, yu, 0),
					vec2(texxleft, texyupper),
					vec3(rgb.r, rgb.g, rgb.b));
			//cout << upperleft << endl;
//
			//cout << endl;

			vertices[vertex++] = lowerleft;
			vertices[vertex++] = lowerright;
			vertices[vertex++] = upperright;
			vertices[vertex++] = upperleft;
		}
	}
	//cout << "used " << vertex << " vertices, for msg: \"" << msg << "\"\n";

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex_t);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(0));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	//return glm::vec2(0.05 * msg.length() + 0.05, 0.1);
	rv.y = yl;
	rv.x = xr;
	this->size = rv;
	return rv;
}

glm::vec2 TextVertices::upload(std::string msg, double scale, unsigned char r, unsigned char g, unsigned char b, float width, float height)
{
	float rf = double(r) / double(0xff);
	float gf = double(g) / double(0xff);
	float bf = double(b) / double(0xff);
	return upload(msg, scale, rf, gf, bf, width, height);
}

/*void TextVertices::draw()
	{
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*projection));
	glm::mat4 tm = glm::translate(view, glm::vec3(pos, 0.0));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(tm));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));


	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

	glDrawArrays(GL_QUADS, 0, vertexCount);

	glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	}*/

void TextVertices::setPosition(glm::vec2 position)
{
	this->pos = position;
}

const glm::vec2 * TextVertices::getPosition()
{
	return &(this->pos);
}

const glm::vec2 * TextVertices::getSize()
{
	return &(this->size);
}

unsigned int TextVertices::getPointer()
{
	return vertexBuffer;
}

unsigned int TextVertices::getVertexCount()
{
	return vertexCount;
}
