#include "textrenderer.hpp"

char * loadFile(const char * path);

TextRenderer::TextRenderer(Window * window)
{
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
	cout << positionAttrib << " " << texcoordAttrib << " " << colorAttrib << endl;

	glUseProgram(programPointer);

	projectionUniform = glGetUniformLocation(programPointer, "projection");
	viewUniform = glGetUniformLocation(programPointer, "view");
	modelUniform = glGetUniformLocation(programPointer, "model");

	view = glm::mat4(1.0);
	model = glm::mat4(1.0);
	projection = window->getProjection();

	//glPolygonMode(GL_FRONT, GL_LINE);

	font = new Image("data/font.png");
	fontTextureUniform = glGetUniformLocation(programPointer, "texture");

	glUniform1i(fontTextureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->getGlPointer());
}

TextRenderer::~TextRenderer()
{
	unloadShader();
}

glm::vec2 TextRenderer::upload(std::string msg, float x, float y, float scale, float r, float g, float b)
{
	using glm::vec3; using glm::vec2;

	glGenBuffers(1, &vertexBuffer);
	vertexCount = msg.length() * 4;
	vertices = new vertex_t[vertexCount];

	const double gap_width = double(TEXTURE_CHAR_GAP) / double(TEXTURE_WIDTH); // the gap width between characters on the texture, in texture (size) units (0 - 1)
	const double char_width = double(TEXTURE_CHAR_WIDTH) / double(TEXTURE_WIDTH);

	double incrx = 0.05 * scale;
	double incry = 0.1 * scale;
	double xl = x, xr = x + incrx;
	double yu = y, yl = y + incry;
	for(unsigned int vertex = 0; vertex < vertexCount; vertex += 4)
	{
		/* Viewport:
		 *     -1 y
		 *       |
		 * -1 x -o- +1 x
		 *    	 |
		 *     +1 y      */
		unsigned char c = msg[vertex / 4];
		double texxleft = 0;
		double texxright = 0;
		double texyupper = 0;
		double texylower = 0;

		vertex_t lowerleft = vertex_t(vec3(xl, yl, 0),
																	vec2((char_width + gap_width) * c, 1.0),
																	vec3(r, g, b));

		vertex_t lowerright = vertex_t(vec3(xr, yl, 0),
																	 vec2(((char_width + gap_width) * c) + char_width, 1.0),
																	 vec3(r, g, b));

		vertex_t upperright = vertex_t(vec3(xr, yu, 0),
																	 vec2(((char_width + gap_width) * c) + char_width, 0.0),
																	 vec3(r, g, b));

		vertex_t upperleft = vertex_t(vec3(xl, yu, 0),
																	vec2((char_width + gap_width) * c, 0.0),
																	vec3(r, g, b));

		vertices[vertex + 0] = lowerleft;
		vertices[vertex + 1] = lowerright;
		vertices[vertex + 2] = upperright;
		vertices[vertex + 3] = upperleft;

		/* check for line breaks caused by long words */
		unsigned int cur_pos = vertex / 4;
		unsigned char cur = 0;
		double xtest = xr;

		/* handle line breaks */
		if(xl > 1 || xr > 1 || c == '\n')
		{
			//reset x values, "cariage return"
			xl = x;
			xr = x + incrx;

			//increment y -> new line
			yu = yu + incry;
			yl = yu + incry;
		}
		else
		{
			xl += incrx;
			xr += incrx;			
		}

	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex_t);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#define _OFFSET(i) ((char *)NULL + (i))
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(0));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vec2(0.05 * msg.length() + 0.05, 0.1);
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
