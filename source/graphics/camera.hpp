#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <iostream>
using namespace std;
#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"

class Camera
{
	public:
	void setPosition (glm::vec3 p);
	void setRotation (glm::vec3 r);
	glm::vec3 getPosition ();
	glm::vec3 getRotation ();

	glm::mat4 getView ();

	public:
	static Camera * getInstance ();
	private:
	Camera();
	~Camera();
	//prevent copying of the singleton
	Camera(Camera const&);
	void operator=(Camera const &);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 view;
	void updateView ();
};

#endif
