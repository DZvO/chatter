#include "textrenderer.hpp"

char * loadFile(const char * path);

TextRenderer::TextRenderer(Window * window)
{
	vertices = NULL;
	programPointer = vertexPointer = fragmentPointer = 0;
	programPointer = glCreateProgram();

	char * vertsource = loadFile("data/textshader.vert");
	vertexPointer = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexPointer, 1, (const char**)(&(vertsource)), NULL);
	glCompileShader(vertexPointer);
	delete [] vertsource;
	glAttachShader(programPointer, vertexPointer);

	char * fragsource = loadFile("data/textshader.frag");
	fragmentPointer = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentPointer, 1, (const char**)(&(fragsource)), NULL);
	glCompileShader(fragmentPointer);
	delete [] fragsource;
	glAttachShader(programPointer, fragmentPointer);

	glLinkProgram(programPointer);

	positionAttrib = glGetAttribLocation(programPointer, "position");
	texcoordAttrib = glGetAttribLocation(programPointer, "texcoord");
	colorAttrib = glGetAttribLocation(programPointer, "color");

	glUseProgram(programPointer);

	projectionUniform = glGetUniformLocation(programPointer, "projection");
	viewUniform = glGetUniformLocation(programPointer, "view");
	modelUniform = glGetUniformLocation(programPointer, "model");

	view = glm::mat4(1.0);
	model = glm::mat4(1.0);
	projection = window->getProjection();

	//glPolygonMode(GL_FRONT, GL_LINE);

	font = new Image("data/font.png");
	kerning = new glm::vec2[256];
	loadKerning();

	fontTextureUniform = glGetUniformLocation(programPointer, "texture");

	glUniform1i(fontTextureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->getGlPointer());
}

TextRenderer::~TextRenderer()
{
	unloadShader();
	delete font;
	delete [] kerning;
	delete [] vertices;
}

glm::vec2 TextRenderer::upload(std::string msg, float x, float y, float scale, float r, float g, float b)
{
	using glm::vec3; using glm::vec2;

	vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	//vertexCount = msg.length() * 4;
	vertexCount = 0;
	for(unsigned int i = 0; msg[i] != '\0'; i++)
	{
		if(msg[i] != ' ')
			vertexCount++;
	}
	vertexCount *= 4;
	if(vertices != NULL)
	{
		delete [] vertices;
	}
	vertices = new vertex_t[vertexCount];

	const double char_width = double(8) / double(128);
	const double pixel_size = 1.0 / double(128);

	double incrx = 0.08 * scale;
	double incry = 0.08 * scale;
	double xl = x, xr = x;// + (0.08 - ((kerning[(unsigned char)msg[0]].x * 0.01) + (kerning[(unsigned char)msg[0]].y * 0.01)));
	double yu = y, yl = y + incry;
	unsigned int vertex = 0;
	double offset = 0.0;
	for(unsigned int i = 0; msg[i] != '\0'; i++)
	{
		 // Viewport:
		 //     -1 y
		 //       |
		 // -1 x -o- +1 x
		 //    	  |
		 //     +1 y     
		unsigned char c = msg[i];

		if(c == ' ')
		{
			xl = xr;
			xr += 0.03;
		}
		else if(c == '\n')// || (xr + (0.08 - ((kerning[c].x * 0.01) + (kerning[c].y * 0.01)) + 0.01)) > 1)
		{
			yu = yl + 0.01;
			yl += incry + 0.01;
			xl = x;
			xr = x;
			offset = 0.0;
		}
		else
		{
			unsigned int c_row = (unsigned int)c / 16;
			unsigned int c_col = (unsigned int)c % 16;

			//line break?
			if(xr + (0.08 - ((kerning[c].x * 0.01) + (kerning[c].y * 0.01))) + 0.01 > 1)
			{
				yu = yl + 0.01;
				yl += incry + 0.01;

				xl = x;
				xr = x + (0.08 - ((kerning[c].x * 0.01) + (kerning[c].y * 0.01)));
				cout << "line break! at char " << c << endl;
			}
			else
			{
				//do kerning stuff
				xl = xr + offset; // 0.01 is a little gap between each character
				offset = 0.01;
				xr += (0.08 - ((kerning[c].x * 0.01) + (kerning[c].y * 0.01))) + 0.01;
				//cout << "kerning for char \"" << c << "\"(" << (unsigned int) c << "), left: " << kerning[c].x << ", right: " << kerning[c].y << endl;
			}


			double texxleft = c_col * char_width;
			texxleft += (kerning[c].x * pixel_size);
			double texxright = c_col * char_width + char_width;
			texxright -= (kerning[c].y * pixel_size);

			double texyupper = c_row * char_width;
			double texylower = c_row * char_width + char_width;

			vertex_t lowerleft = vertex_t(vec3(xl, yl, 0),
					vec2(texxleft, texylower),
					vec3(r, g, b));

			vertex_t lowerright = vertex_t(vec3(xr, yl, 0),
					vec2(texxright, texylower),
					vec3(r, g, b));

			vertex_t upperright = vertex_t(vec3(xr, yu, 0),
					vec2(texxright, texyupper),
					vec3(r, g, b));

			vertex_t upperleft = vertex_t(vec3(xl, yu, 0),
					vec2(texxleft, texyupper),
					vec3(r, g, b));

			vertices[vertex++] = lowerleft;
			vertices[vertex++] = lowerright;
			vertices[vertex++] = upperright;
			vertices[vertex++] = upperleft;
		}
	}
	cout << "used " << vertex << " vertices" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex_t);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(0));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//return glm::vec2(0.05 * msg.length() + 0.05, 0.1);
	return glm::vec2(0, 0);
}

void TextRenderer::draw()
{
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*projection));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

	glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);
	glDrawArrays(GL_QUADS, 0, vertexCount);
	glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
}

//private
void TextRenderer::loadKerning()
{
	for(unsigned int row = 0; row <= 15; row++)
	{
		for(unsigned int col = 0; col <= 15; col++)
		{
			kerning[(row * 16) + col] = glm::vec2(0, 0); // initialize array
			const unsigned int cellW = 8;//TEXTURE_WH / 16; //default = 8
			const unsigned int cellH = 8;//TEXTURE_WH / 16; //default = 8
			//cout << "char (" << (row*16)+col << ")\n";

			//left to right
			for(unsigned int pixelX = 0; pixelX < 8; pixelX++)
			{
				for(unsigned int pixelY = 0; pixelY < 8; pixelY++)
				{
					if(font->getPixel((col * 8) + pixelX, (row * 8) + pixelY) != 0)
					{
						//cout << "from left in cell @ [" << row << ", " << col << "] pixel at [" << pixelX << ", " << pixelY << "] is not empty! (absolute: [" << (col*8)+pixelX << ", " << (row*8)+pixelY << "]" << endl;
						kerning[(row * 16) + col].x = pixelX;
						//break loops
						pixelX = 8;
						pixelY = 8;
					}
				}
			}

			//right to left
			for(int pixelX = 7; pixelX >= 0; pixelX--)
			{
				for(int pixelY = 7; pixelY >= 0; pixelY--)
				{
					if(font->getPixel((col * 8) + pixelX, (row * 8) + pixelY) != 0)
					{
						//cout << "from right in cell @ [" << row << ", " << col << "] pixel at [" << pixelX << ", " << pixelY << "] is not empty! (absolute: [" << (col*8)+pixelX << ", " << (row*8)+pixelY << "]" << endl;
						kerning[(row * 16) + col].y = 7 - pixelX;
						//break loops
						pixelX = -1;
						pixelY = -1;
					}
				}
			}
			//cout << endl;
		}
	}
}

char * loadFile (const char * path)
{
	FILE *fd;
	long len, r;
	char *str;

	if (!(fd = fopen(path, "r")))
	{
		fprintf(stderr, "Can't open file '%s' for reading\n", path);
		return NULL;
	}

	fseek(fd, 0, SEEK_END); // set position pointer to file end
	len = ftell(fd); // get the count of bytes that the pointer went
	fseek(fd, 0, SEEK_SET); // reset the pointer to the beginning of the file

	if (!(str = (char*)malloc(len * sizeof(char))))
	{
		fprintf(stderr, "Can't malloc space for '%s'\n", path);
		return NULL;
	}

	r = fread(str, sizeof(char), len, fd);

	str[r - 1] = '\0'; //make sure the string ends

	fclose(fd);

	return str;
}

bool shaderError(unsigned int object, unsigned int status)
{
	int ok = 0;
	if(status == GL_LINK_STATUS)
	{
		glGetProgramiv(object, status, &ok);
		return !ok;
	}
	else if(status == GL_COMPILE_STATUS)
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &ok);
		return !ok;
	}
	return true;
}

void show_error(unsigned int object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	int length;
	char *out;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &length);
	out = new char[length];
	glGet__InfoLog(object, length, NULL, out);
	std::cout << out << endl;
	delete [] out;
}


void TextRenderer::loadShader()
{
	programPointer = glCreateProgram();

	vertexPointer = glCreateShader(GL_VERTEX_SHADER);
	char * vertexShader = loadFile("data/textshader.vert");
	glShaderSource(vertexPointer, 1, (const char**)(&(vertexShader)), NULL);
	glCompileShader(vertexPointer);

	fragmentPointer = glCreateShader(GL_FRAGMENT_SHADER);
	char * fragmentShader = loadFile("data/textshader.frag");
	glShaderSource(fragmentPointer, 1, (const char**)(&(fragmentShader)), NULL);
	glCompileShader(fragmentPointer);

	if(shaderError(fragmentPointer, GL_COMPILE_STATUS))
	{
		std::cout << "error in textshader.frag" << std::endl;
		show_error(fragmentPointer, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(fragmentPointer);
		return;
	}
	if(shaderError(vertexPointer, GL_COMPILE_STATUS))
	{
		std::cout << "error in textshader.vert" << std::endl;
		show_error(vertexPointer, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(vertexPointer);
		return;
	}

	glAttachShader(programPointer, vertexPointer);
	glAttachShader(programPointer, fragmentPointer);
	//glDeleteShader(vertexPointer);
	//glDeleteShader(fragmentPointer);
	glLinkProgram(programPointer);

	if(shaderError(programPointer, GL_LINK_STATUS))
	{
		std::cout << "error while linking :(" << std::endl;
		show_error(programPointer, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(programPointer);
		return;
	}

	delete [] fragmentShader;
	delete [] vertexShader;
}

void TextRenderer::unloadShader()
{
	glUseProgram(0);
	glDeleteShader(fragmentPointer);
	glDeleteShader(vertexPointer);
	glDeleteProgram(programPointer);
}
