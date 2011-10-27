#include "cube.hpp"

char * Cube::loadFile(const char* path)
{
	FILE *fd;	long len, r; char *str;
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

Cube::Cube() : rotation(0.0), position(0.0, 0.0, -15.0), modelmatrix(1.0)
{
	texture = new Image("data/cube.png");
	texture->upload();

	programPointer = vertexPointer = fragmentPointer = 0;
	positionAttrib = texcoordAttrib = colorAttrib = 0;
	projectionUniform = viewUniform = modelUniform = texUniform = 0;

	programPointer = glCreateProgram();

	char * vertSource = loadFile("data/base3d.vert");
	vertexPointer = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexPointer, 1, (const char**)(&(vertSource)), NULL);
	glCompileShader(vertexPointer);
	delete [] vertSource;
	glAttachShader(programPointer, vertexPointer);

	char * fragSource = loadFile("data/base3d.frag");
	fragmentPointer = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentPointer, 1, (const char**)(&(fragSource)), NULL);
	glCompileShader(fragmentPointer);
	delete [] fragSource;
	glAttachShader(programPointer, fragmentPointer);
	
	glLinkProgram(programPointer);

	glUseProgram(programPointer);
	glUseProgram(0);

	positionAttrib = glGetAttribLocation(programPointer, "position");
	texcoordAttrib = glGetAttribLocation(programPointer, "texcoord");
	colorAttrib = glGetAttribLocation(programPointer, "color");

	projectionUniform = glGetUniformLocation(programPointer, "projection");
	viewUniform = glGetUniformLocation(programPointer, "view");
	modelUniform = glGetUniformLocation(programPointer, "model");
	texUniform = glGetUniformLocation(programPointer, "texture");

	//vertices
	vertexCount = 24;
	glGenBuffers(1, &vertexBuffer);
	vertices = new vertex_t [vertexCount];

	using glm::vec3; using glm::vec2;

	//middle/center of cube is (0|0|0)
	const float s = 1.0 / 6.0 - 0.0001;
	
	//front face - 1st part of texture
	vertices[0]  = vertex_t(vec3(-0.5, +0.5, +0.5), vec2(0, 0), vec3(1.0, 1.0, 1.0));//upper - left - front 
	vertices[1]  = vertex_t(vec3(-0.5, -0.5, +0.5), vec2(0, 1), vec3(1.0, 1.0, 1.0));//lower - left - front
	vertices[2]  = vertex_t(vec3(+0.5, -0.5, +0.5), vec2(s, 1), vec3(1.0, 1.0, 1.0));//lower - right - front 
	vertices[3]  = vertex_t(vec3(+0.5, +0.5, +0.5), vec2(s, 0), vec3(1.0, 1.0, 1.0));//upper - right - front

	//right face - 2nd part of texture
	vertices[4]  = vertex_t(vec3(+0.5, +0.5, +0.5), vec2(s, 0), vec3(1.0, 1.0, 1.0));//upper - left - right
	vertices[5]  = vertex_t(vec3(+0.5, -0.5, +0.5), vec2(s, 1), vec3(1.0, 1.0, 1.0));//lower - left - right
	vertices[6]  = vertex_t(vec3(+0.5, -0.5, -0.5), vec2(s*2, 1), vec3(1.0, 1.0, 1.0));//lower - right - right 
	vertices[7]  = vertex_t(vec3(+0.5, +0.5, -0.5), vec2(s*2, 0), vec3(1.0, 1.0, 1.0));//upper - right - right

	//back face - 3rd part of texture
	vertices[8]  = vertex_t(vec3(+0.5, +0.5, -0.5), vec2(s*2, 0), vec3(1.0, 1.0, 1.0));//upper - left - back
	vertices[9]  = vertex_t(vec3(+0.5, -0.5, -0.5), vec2(s*2, 1), vec3(1.0, 1.0, 1.0));//lower - left - back
	vertices[10] = vertex_t(vec3(-0.5, -0.5, -0.5), vec2(s*3, 1), vec3(1.0, 1.0, 1.0));//lower - right - back
	vertices[11] = vertex_t(vec3(-0.5, +0.5, -0.5), vec2(s*3, 0), vec3(1.0, 1.0, 1.0));//upper - right - back

	//left face - 4th part of texture
	vertices[12] = vertex_t(vec3(-0.5, +0.5, -0.5), vec2(s*3, 0), vec3(1.0, 1.0, 1.0));//upper - left - left
	vertices[13] = vertex_t(vec3(-0.5, -0.5, -0.5), vec2(s*3, 1), vec3(1.0, 1.0, 1.0));//lower - left - left
	vertices[14] = vertex_t(vec3(-0.5, -0.5, +0.5), vec2(s*4, 1), vec3(1.0, 1.0, 1.0));//lower - right - left
	vertices[15] = vertex_t(vec3(-0.5, +0.5, +0.5), vec2(s*4, 0), vec3(1.0, 1.0, 1.0));//upper - right - left

	//top face - 5th part of texture
	vertices[16] = vertex_t(vec3(-0.5, +0.5, -0.5), vec2(s*4, 0), vec3(1.0, 1.0, 1.0));//upper - left - top
	vertices[17] = vertex_t(vec3(-0.5, +0.5, +0.5), vec2(s*4, 1), vec3(1.0, 1.0, 1.0));//lower - left - top
	vertices[18] = vertex_t(vec3(+0.5, +0.5, +0.5), vec2(s*5, 1), vec3(1.0, 1.0, 1.0));//lower - right - top
	vertices[19] = vertex_t(vec3(+0.5, +0.5, -0.5), vec2(s*5, 0), vec3(1.0, 1.0, 1.0));//upper - right - top

	//bottom face - 6th part of texture
	vertices[20] = vertex_t(vec3(-0.5, -0.5, +0.5), vec2(s*5, 0), vec3(1.0, 1.0, 1.0));//upper - left - bottom
	vertices[21] = vertex_t(vec3(-0.5, -0.5, -0.5), vec2(s*5, 1), vec3(1.0, 1.0, 1.0));//lower - left - bottom
	vertices[22] = vertex_t(vec3(+0.5, -0.5, -0.5), vec2(1, 1), vec3(1.0, 1.0, 1.0));//lower - right - bottom
	vertices[23] = vertex_t(vec3(+0.5, -0.5, +0.5), vec2(1, 0), vec3(1.0, 1.0, 1.0));//upper - right - bottom


	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex_t);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::tick ()
{
	rotation.y -= 1.8 * (Window::getInstance()->getFrametime() / 10) / 10;
	rotation.x += 1.0 * (Window::getInstance()->getFrametime() / 10) / 10;
}

void Cube::draw ()
{
	glUseProgram(programPointer);
	glUniform1i(texUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getGlPointer());

	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getPerspectiveProjection())));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

	glm::mat4 translated_model = modelmatrix;
	translated_model = glm::translate(translated_model, position);
	translated_model = glm::scale(translated_model, glm::vec3(4.0, 4.0, 4.0));
	translated_model = glm::rotate(translated_model, rotation.x, glm::vec3(1, 0, 0));
	translated_model = glm::rotate(translated_model, rotation.y, glm::vec3(0, 1, 0));
	translated_model = glm::rotate(translated_model, rotation.z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(translated_model));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(0));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

	glDrawArrays(GL_QUADS, 0, vertexCount);

	glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
