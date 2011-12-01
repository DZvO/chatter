#include "camera.hpp"

Camera * Camera::getInstance ()
{
	static Camera instance;
	return &instance;
}

Camera::Camera ()
{
	position = rotation = glm::vec3(0.0);
	view = glm::mat4(1.0);
}

Camera::~Camera ()
{
}

void Camera::translate (glm::vec3 delta)
{
	glm::vec4 dv = glm::vec4(delta, 1.0) * view;
	//view = glm::translate(view, glm::vec3(dv));
	position += glm::vec3(dv);
	updateView();
}

void Camera::rotate (glm::vec3 delta)
{
	rotation += delta;
	//view = glm::translate(view, position);
	//view = glm::rotate(view, delta.x, glm::vec3(1, 0, 0));
	//view = glm::rotate(view, delta.y, glm::vec3(0, 1, 0));
	//view = glm::rotate(view, delta.z, glm::vec3(0, 0, 1));
	updateView();
}

void Camera::setPosition (glm::vec3 p)
{
	position = p;
	updateView();
}

void Camera::setRotation (glm::vec3 r)
{
	rotation = r;
	updateView();
}

glm::vec3 Camera::getPosition ()
{
	return position;
}

glm::vec3 Camera::getRotation ()
{
	return rotation;
}

glm::mat4 Camera::getView ()
{
	return view;
}

void Camera::updateView ()
{
	view = glm::mat4(1.0);
	view = glm::rotate(view, rotation.x, glm::vec3(1, 0, 0));
	view = glm::rotate(view, rotation.y, glm::vec3(0, 1, 0));
	view = glm::rotate(view, rotation.z, glm::vec3(0, 0, 1));
	view = glm::translate(view, position);
}
