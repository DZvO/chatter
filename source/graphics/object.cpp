#include "object.hpp"

Object::Object ()
{
	vertexBuffer = vertexCount = type = 0;
	vertices = nullptr;
	texture = nullptr;
	modelMatrix = glm::mat4(1.0);
	position = rotation = glm::vec3(0.0);
	size = glm::vec3(1.0);
}

Object::~Object ()
{
	delete texture;
	delete [] vertices;
}

void Object::setPosition (glm::vec3 p)
{
	position = p;
}

void Object::setSize (glm::vec3 s)
{
	size = s;
}

char * Object::loadFile(const char* path)
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

void Object::uploadVertexBuffer ()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLsizeiptr const vertexSize = vertexCount * sizeof(vertex);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
