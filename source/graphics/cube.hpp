#ifndef _CUBE_HPP
#define _CUBE_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glew/glew.h"
#include <GL/gl.h>
#include "graphics/window.hpp"
using motor::Window;
#include "graphics/camera.hpp"
#include "graphics/image.hpp"
#include "graphics/shaderHelper.hpp"

class Cube
{
	public:
	class vertex_t
	{
		public:
		glm::vec3 pos;//world position
		glm::vec3 normal;
		glm::vec2 tex;//texture coordinates
		glm::vec3 col;
		vertex_t() {}
		vertex_t(glm::vec3 position, glm::vec2 texcoord) : pos(position), tex(texcoord), col(1.0) {}
		vertex_t(glm::vec3 position, glm::vec2 texcoord, glm::vec3 color) : pos(position), tex(texcoord), col(color) {}
		friend ostream& operator<<(ostream& os, const vertex_t& vt)
		{
			cout << "pos(" << vt.pos.x << "|" << vt.pos.y << "), tex(" << vt.tex.x << "|" << vt.tex.y << ")";
			return os;
		}
		static glm::vec3 cross (glm::vec3 a, glm::vec3 b)
		{
			glm::vec3 result;
			result.x = a.y * b.z - a.z * b.y;
			result.y = a.z * b.x - a.x * b.z;
			result.z = a.x * b.y - a.y * b.x;
			return result;
		}

		static glm::vec3 getTriangleNormal (glm::vec3 a, glm::vec3 b, glm::vec3 c)
		{
			glm::vec3 edge1 = b - a;
			glm::vec3 edge2 = c - a;
			glm::vec3 normal = cross(edge1, edge2);
			return normal;
		}
	};

	public: 
	Cube (bool useLight = true);
	void tick ();
	void draw ();

	void setPosition (glm::vec3 p);
	void setSize (glm::vec3 s);

	public:
	char * loadFile (const char * path);
	Image * texture;
	glm::vec3 rotation;
	glm::vec3 position;
	glm::vec3 size;
	glm::mat4 modelmatrix;

	glm::mat4 getModelMatrix ()
	{
		glm::mat4 translated_model = modelmatrix;
		translated_model = glm::translate(translated_model, position);
		translated_model = glm::scale(translated_model, size);
		translated_model = glm::rotate(translated_model, rotation.x, glm::vec3(1, 0, 0));
		translated_model = glm::rotate(translated_model, rotation.y, glm::vec3(0, 1, 0));
		translated_model = glm::rotate(translated_model, rotation.z, glm::vec3(0, 0, 1));
		return translated_model;
	}

	unsigned int programPointer, vertexPointer, fragmentPointer;
	unsigned int positionAttrib, normalAttrib, texcoordAttrib, colorAttrib;
	unsigned int projectionUniform, viewUniform, modelUniform, texUniform;

	unsigned int num_lights;
	unsigned int programLightPosLocation, programLightColorLocation;
	float lightColor[3];
	float lightPosition[3];

	unsigned int vertexBuffer, vertexCount;
	vertex_t * vertices;
};

#endif
