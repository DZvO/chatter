#include "textrenderer.hpp"

char * loadFile(const char * path);

TextRenderer::TextRenderer()
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

	glUseProgram(programPointer);

	projectionUniform = glGetUniformLocation(programPointer, "projection");
	viewUniform = glGetUniformLocation(programPointer, "view");
	modelUniform = glGetUniformLocation(programPointer, "model");

	projection = glm::perspective(80.0, double(800) / double(600), 0.1, 1000.0);
	view = glm::mat4(1.0);
	model = glm::mat4(1.0);

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

void TextRenderer::upload(std::string msg, float x, float y, float scale)
{
	//vertexCount = msg.length() * 4;
	//vertices = new vertex_t [vertexCount];
	float z = -10;
	scale = 1 / scale;

	glGenBuffers(1, &vertexBuffer);

	vertexCount = msg.length() * 4;
	vertices = new vertex_t[vertexCount];
	double current_char_offset = 0;
	for(int vertex = 0, current_char = 0; vertex < vertexCount; vertex += 4)
	{
		unsigned char c = msg[current_char];
		float gap_width = double(TEXTURE_CHAR_GAP) / double(TEXTURE_WIDTH);
		double pixel_width = 1.0 / double(TEXTURE_WIDTH);
		float char_width = double(TEXTURE_CHAR_WIDTH) / double(TEXTURE_WIDTH);

		vertex_t lowerleft = vertex_t(glm::vec3(0 + current_char_offset + x, 0 + y, z * scale), glm::vec2((char_width + gap_width) * c, 1.0));
		vertices[vertex+0] = lowerleft;

		vertex_t lowerright = vertex_t(glm::vec3(1 + current_char_offset + x - 0.5, 0 + y, z * scale), glm::vec2(((char_width + gap_width) * c) + char_width, 1.0));
		vertices[vertex+1] = lowerright;

		vertex_t upperright = vertex_t(glm::vec3(1 + current_char_offset + x - 0.5, 1.0 + y, z * scale), glm::vec2(((char_width + gap_width) * c) + char_width, 0.0));
		vertices[vertex+2] = upperright;

		vertex_t upperleft = vertex_t(glm::vec3(0 + current_char_offset + x, 1.0 + y, z * scale), glm::vec2((char_width + gap_width) * c, 0.0));
		vertices[vertex+3] = upperleft;

		current_char_offset += 0.50;
		current_char++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex_t);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#define _OFFSET(i) ((char *)NULL + (i))
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(0));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::draw()
{
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

	glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib);
	glDrawArrays(GL_QUADS, 0, vertexCount);
	glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(texcoordAttrib);
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
