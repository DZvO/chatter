#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include <iostream>
#include "lib/glm/glm.hpp"

#include "graphics/image.hpp"

class Object
{
	public:
		class vertex
		{
			public:
				glm::vec3 pos;//world position
				glm::vec3 normal;
				glm::vec2 tex;//texture coordinates
				glm::vec3 col;
				vertex () {}
				vertex (glm::vec3 position, glm::vec2 texcoord) : pos(position), tex(texcoord), col(1.0) {}
				vertex (glm::vec3 position, glm::vec2 texcoord, glm::vec3 color) : pos(position), tex(texcoord), col(color) {}

				friend std::ostream& operator<< (std::ostream& os, const vertex& vt)
				{
					std::cout << "pos(" << vt.pos.x << "|" << vt.pos.y << "), tex(" << vt.tex.x << "|" << vt.tex.y << ")";
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
		Object ();
		~Object ();

		void loadTexture (std::string path);

		void setPosition (glm::vec3 p);
		void setSize (glm::vec3 s);

		glm::mat4 getModelMatrix ();

	protected:
		unsigned int vertexBuffer, vertexCount;
		unsigned int type;
		vertex * vertices;
		glm::mat4 modelMatrix;
		glm::vec3 position, rotation, size;
		Image * texture;

		void updateModelMatrix();
		void uploadVertexBuffer ();
		char * loadFile (const char * path);
};

#endif
