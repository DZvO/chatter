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

bool error(unsigned int object, unsigned int status)
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
	cout << out << endl;
	delete [] out;
}


Cube::Cube(bool useLight) : rotation(0.0), position(0.0, 0.0, -15.0), size(4, 4, 4), modelmatrix(1.0)
{
	num_lights = 1;

	texture = new Image("data/cube.png");
	texture->upload();

	//programPointer = vertexPointer = fragmentPointer = 0;
	positionAttrib = texcoordAttrib = colorAttrib = 0;
	projectionUniform = viewUniform = modelUniform = texUniform = 0;

	if(useLight)
		programPointer = ShaderHelper::loadShader("data/fragmentLight.vert", "data/fragmentLight.frag");
	else
		programPointer = ShaderHelper::loadShader("data/base3d.vert", "data/base3d.frag");

	positionAttrib = glGetAttribLocation(programPointer, "position");
	normalAttrib = glGetAttribLocation(programPointer, "normal");
	texcoordAttrib = glGetAttribLocation(programPointer, "texcoord");
	colorAttrib = glGetAttribLocation(programPointer, "color");

	projectionUniform = glGetUniformLocation(programPointer, "projection");
	viewUniform = glGetUniformLocation(programPointer, "view");
	modelUniform = glGetUniformLocation(programPointer, "model");
	texUniform = glGetUniformLocation(programPointer, "texture");

	programLightPosLocation = glGetUniformLocation(programPointer, "lightPos");
	//programLightColorLocation = glGetUniformLocation(programPointer, "lightColor");

	lightColor[0] = 1.0f; lightColor[1] = 1.0f; lightColor[2] = 1.0f;
	lightPosition[0] = 6.0f; lightPosition[1] = 7.0f; lightPosition[2] = -8.0f;

	//vertices
	vertexCount = 4*6;
	glGenBuffers(1, &vertexBuffer);
	vertices = new vertex_t [vertexCount];

	using glm::vec3; using glm::vec2;

	//middle/center of cube is (0|0|0)
	const double s = 1.0 / 6.0;
	const double t = 0.0;
	const double b = 1.0;

	//front face - 1st part of texture
	vertices[0]  = vertex_t(vec3(-0.5, +0.5, +0.5), vec2(0, t), vec3(1.0, 1.0, 1.0));//upper - left - front 
	vertices[1]  = vertex_t(vec3(-0.5, -0.5, +0.5), vec2(0, b), vec3(1.0, 1.0, 1.0));//lower - left - front
	vertices[2]  = vertex_t(vec3(+0.5, -0.5, +0.5), vec2(s, b), vec3(1.0, 1.0, 1.0));//lower - right - front 
	vertices[3]  = vertex_t(vec3(+0.5, +0.5, +0.5), vec2(s, t), vec3(1.0, 1.0, 1.0));//upper - right - front
	vertices[0].normal = vertices[1].normal = vertices[2].normal = vertices[3].normal = vertex_t::getTriangleNormal(vertices[0].pos, vertices[1].pos, vertices[2].pos);

	//right face - 2nd part of texture
	vertices[4]  = vertex_t(vec3(+0.5, +0.5, +0.5), vec2(s, t), vec3(1.0, 1.0, 1.0));//upper - left - right
	vertices[5]  = vertex_t(vec3(+0.5, -0.5, +0.5), vec2(s, b), vec3(1.0, 1.0, 1.0));//lower - left - right
	vertices[6]  = vertex_t(vec3(+0.5, -0.5, -0.5), vec2(s*2, b), vec3(1.0, 1.0, 1.0));//lower - right - right 
	vertices[7]  = vertex_t(vec3(+0.5, +0.5, -0.5), vec2(s*2, t), vec3(1.0, 1.0, 1.0));//upper - right - right
	vertices[4].normal = vertices[5].normal = vertices[6].normal = vertices[7].normal = vertex_t::getTriangleNormal(vertices[4].pos, vertices[5].pos, vertices[6].pos);

	//back face - 3rd part of texture
	vertices[8]  = vertex_t(vec3(+0.5, +0.5, -0.5), vec2(s*2, t), vec3(1.0, 1.0, 1.0));//upper - left - back
	vertices[9]  = vertex_t(vec3(+0.5, -0.5, -0.5), vec2(s*2, b), vec3(1.0, 1.0, 1.0));//lower - left - back
	vertices[10] = vertex_t(vec3(-0.5, -0.5, -0.5), vec2(s*3, b), vec3(1.0, 1.0, 1.0));//lower - right - back
	vertices[11] = vertex_t(vec3(-0.5, +0.5, -0.5), vec2(s*3, t), vec3(1.0, 1.0, 1.0));//upper - right - back
	vertices[8].normal = vertices[9].normal = vertices[10].normal = vertices[11].normal = vertex_t::getTriangleNormal(vertices[8].pos, vertices[9].pos, vertices[10].pos);

	//left face - 4th part of texture
	vertices[12] = vertex_t(vec3(-0.5, +0.5, -0.5), vec2(s*3, t), vec3(1.0, 1.0, 1.0));//upper - left - left
	vertices[13] = vertex_t(vec3(-0.5, -0.5, -0.5), vec2(s*3, b), vec3(1.0, 1.0, 1.0));//lower - left - left
	vertices[14] = vertex_t(vec3(-0.5, -0.5, +0.5), vec2(s*4, b), vec3(1.0, 1.0, 1.0));//lower - right - left
	vertices[15] = vertex_t(vec3(-0.5, +0.5, +0.5), vec2(s*4, t), vec3(1.0, 1.0, 1.0));//upper - right - left
	vertices[12].normal = vertices[13].normal = vertices[14].normal = vertices[15].normal = vertex_t::getTriangleNormal(vertices[12].pos, vertices[13].pos, vertices[14].pos);

	//top face - 5th part of texture
	vertices[16] = vertex_t(vec3(-0.5, +0.5, -0.5), vec2(s*4, t), vec3(1.0, 1.0, 1.0));//upper - left - top
	vertices[17] = vertex_t(vec3(-0.5, +0.5, +0.5), vec2(s*4, b), vec3(1.0, 1.0, 1.0));//lower - left - top
	vertices[18] = vertex_t(vec3(+0.5, +0.5, +0.5), vec2(s*5, b), vec3(1.0, 1.0, 1.0));//lower - right - top
	vertices[19] = vertex_t(vec3(+0.5, +0.5, -0.5), vec2(s*5, t), vec3(1.0, 1.0, 1.0));//upper - right - top
	vertices[16].normal = vertices[17].normal = vertices[18].normal = vertices[19].normal = vertex_t::getTriangleNormal(vertices[16].pos, vertices[17].pos, vertices[18].pos);

	//bottom face - 6th part of texture
	vertices[20] = vertex_t(vec3(-0.5, -0.5, +0.5), vec2(s*5, t), vec3(1.0, 1.0, 1.0));//upper - left - bottom
	vertices[21] = vertex_t(vec3(-0.5, -0.5, -0.5), vec2(s*5, b), vec3(1.0, 1.0, 1.0));//lower - left - bottom
	vertices[22] = vertex_t(vec3(+0.5, -0.5, -0.5), vec2(1, b), vec3(1.0, 1.0, 1.0));//lower - right - bottom
	vertices[23] = vertex_t(vec3(+0.5, -0.5, +0.5), vec2(1, t), vec3(1.0, 1.0, 1.0));//upper - right - bottom
	vertices[20].normal = vertices[21].normal = vertices[22].normal = vertices[23].normal = vertex_t::getTriangleNormal(vertices[20].pos, vertices[21].pos, vertices[22].pos);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex_t);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::tick ()
{
	rotation.y -= 3.5 * (Window::getInstance()->getFrametime() / 10) / 10;
	rotation.x += 4.0 * (Window::getInstance()->getFrametime() / 10) / 10;
}

void Cube::draw ()
{
	glUseProgram(programPointer);
	glUniform1i(texUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getGlPointer());

	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getPerspectiveProjection())));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(Camera::getInstance()->getView()));

	glm::mat4 translated_model = modelmatrix;
	translated_model = glm::translate(translated_model, position);
	translated_model = glm::scale(translated_model, size);
	translated_model = glm::rotate(translated_model, rotation.x, glm::vec3(1, 0, 0));
	translated_model = glm::rotate(translated_model, rotation.y, glm::vec3(0, 1, 0));
	translated_model = glm::rotate(translated_model, rotation.z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(translated_model));

	glUniform3fv(programLightPosLocation, num_lights, lightPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(0));
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(normalAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

	glDrawArrays(GL_QUADS, 0, vertexCount);

	glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(normalAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Cube::setPosition (glm::vec3 p)
{
	position = p;
}

void Cube::setSize (glm::vec3 s)
{
	size = s;
}
