#ifndef _SCENEMANAGER_HPP
#define _SCENEMANAGER_HPP

#include <iostream>
#include <vector>
#include <list>
#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glew/glew.h"
#include <GL/gl.h>
#include "graphics/cube.hpp"
#include "graphics/shaderHelper.hpp"

class SceneManager
{
	public:
		SceneManager ();
		~SceneManager ();
		void addLight (glm::vec3 pos, glm::vec3 col);
		void addObject (Cube * object);

		void draw ();
	private:

		unsigned int programPointer;//, vertexPointer, fragmentPointer;
		unsigned int positionAttrib, normalAttrib, texcoordAttrib, colorAttrib;
		unsigned int projectionUniform, viewUniform, modelUniform, texUniform;

		unsigned int lightCount;
		unsigned int lightPosUniform, lightColUniform, lightCountUniform;
		std::vector<glm::vec3> lightPos;
		std::vector<glm::vec3> lightCol;
		//std::vector<unsigned int> lightPosUniform;
		//std::vector<unsigned int> lightColUniform;

		std::vector<Cube*> objects;
};

#endif
