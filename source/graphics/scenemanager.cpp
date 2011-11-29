#include "scenemanager.hpp"
SceneManager::SceneManager ()
{
	programPointer = ShaderHelper::loadShader("data/fragmentLight.vert", "data/fragmentLight.frag");
	positionAttrib = texcoordAttrib = colorAttrib = 0;
	projectionUniform = viewUniform = modelUniform = texUniform = 0;

	if(true)
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

	lightPosUniform = glGetUniformLocation(programPointer, "lightPos");
	lightColUniform = glGetUniformLocation(programPointer, "lightCol");
	lightCountUniform = glGetUniformLocation(programPointer, "lightCount");

}

SceneManager::~SceneManager ()
{
}

void SceneManager::addLight (glm::vec3 pos, glm::vec3 col)
{
	lightCount++;
	lightPos.push_back(pos);
	lightCol.push_back(col);
}

void SceneManager::addObject (Cube * object)
{
	objects.push_back(object);
}

void SceneManager::draw ()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(programPointer);

	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getPerspectiveProjection())));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(Camera::getInstance()->getView()));

	glUniform1i(lightCountUniform, lightCount);
	glUniform3fv(lightPosUniform, lightCount, glm::value_ptr(lightPos[0]));
	glUniform3fv(lightColUniform, lightCount, glm::value_ptr(lightCol[0]));

	for(Cube * object : objects)
	{
	glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Cube::vertex_t), (void*)(0));
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Cube::vertex_t), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Cube::vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Cube::vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuffer);
		glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(normalAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

		glUniform1i(texUniform, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, object->texture->getGlPointer());

		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(object->getModelMatrix()));

		glDrawArrays(GL_QUADS, 0, object->vertexCount);
	}

	glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(normalAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
