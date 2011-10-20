#ifndef _CUBE_HPP
#define _CUBE_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "lib/glew/glew.h"
#include <GL/gl.h>

class Cube
{
	public: 
	void tick ();

	Cube() : rotation(0.0), position(0.0, 0.0, -15.0)
	{}
	private:
	float rotation;
	glm::vec3 position;
};

#endif
