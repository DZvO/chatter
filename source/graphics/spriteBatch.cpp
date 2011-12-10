#include "spriteBatch.hpp"
unsigned int SpriteBatch::programPointer = 0;
unsigned int SpriteBatch::positionAttrib = 0;
unsigned int SpriteBatch::texcoordAttrib = 0;
unsigned int SpriteBatch::colorAttrib = 0;
unsigned int SpriteBatch::projectionUniform = 0;
unsigned int SpriteBatch::viewUniform = 0;
unsigned int SpriteBatch::modelUniform = 0;
unsigned int SpriteBatch::texUniform = 0;
unsigned int SpriteBatch::mvpUniform = 0;

SpriteBatch::SpriteBatch ()
{
	programPointer = ShaderHelper::loadShader("data/spritebatch.vert", "data/spritebatch.frag");

	positionAttrib = glGetAttribLocation(programPointer, "position");
	texcoordAttrib = glGetAttribLocation(programPointer, "texcoord");
	colorAttrib = glGetAttribLocation(programPointer, "color");

	//projectionUniform = glGetUniformLocation(programPointer, "projection"); //TODO maybe calculate the mvp on the cpu instead of the gpu so we don't have that much bus traffic? TODO
	//viewUniform = glGetUniformLocation(programPointer, "view");
	//modelUniform = glGetUniformLocation(programPointer, "model");
	mvpUniform = glGetUniformLocation(programPointer, "mvp");
	texUniform = glGetUniformLocation(programPointer, "texture");
}

void SpriteBatch::begin ()
{
}

inline void SpriteBatch::addVertex (unsigned int texId, const short & x, const short & y, const float & texX, const float & texY, unsigned int color)
{
	auto * vertexInfo = &vertices[texId];
	vertex_t * vert = &vertexInfo->vertices[vertexInfo->vertexCount];
	vertexInfo->vertexCount++;

	vert->position[0] = x;
	vert->position[1]= y;
	vert->uv[0] = texX;
	vert->uv[1] = texY;
	vert->color = color;
}

void SpriteBatch::draw (const Image & texture, const Vector2 & position)
{
	draw(texture, Rectangle(position.x, position.y, texture.getWidth(), texture.getHeight()));
}

//Get everything in pixels, later the ortho-projection will have screen values in it so we can use shorts ~~ confused?
void SpriteBatch::draw (const Image & texture, const Rectangle & destination, const Rectangle & uv, const Vector4 & color, const float & rotation, const Vector2 & origin, float scale, const float & layerDepth)
{
	unsigned int texid = texture.getId();
	auto it = vertices.find(texid);
	if(it == vertices.end()) //element doesn't exist yet
		vertices[texid].vertexCount = 0; // -> create it!

	Rectangle realuv = uv;
	if(realuv.x == 0 && realuv.y == 0 && realuv.width == 0 && realuv.height == 0)
	{
		realuv.width = texture.getWidth();
		realuv.height = texture.getHeight();
	}

	//convert pixels in uv to 'real' uv coordinate (0 ... 1)
	double uvxmin = (1.0 / texture.getWidth()) * realuv.x;
	double uvxmax = (1.0 / texture.getWidth()) * (realuv.x + realuv.width);
	double uvymin = (1.0 / texture.getHeight()) * realuv.y;
	double uvymax = (1.0 / texture .getHeight()) * (realuv.y + realuv.height);

	//abgr
	unsigned int packed_color = ((unsigned char)(color.a * 255.0f) << 24) | ((unsigned char)(color.b * 255.0f) << 16) | ((unsigned char)(color.g * 255.0f) << 8) | (unsigned char)(color.r * 255.0f);

	/*
	 *			/2
	 *		 / |
	 *    /  |
	 *   0---1
	 */
	//addVertex(texid, destination.x, destination.y + destination.height, uvxmin, uvymax, packed_color); //0
	//addVertex(texid, destination.x + destination.width, destination.y + destination.height, uvxmax, uvymax, packed_color); //1
	//addVertex(texid, destination.x + destination.width, destination.y, uvxmax, uvymin, packed_color); //2

	/*
	 *   1---0
	 *   |  /
	 *   | /
	 *   2/
	 */
	//addVertex(texid, destination.x + destination.width, destination.y, uvxmax, uvymin, packed_color); //0
	//addVertex(texid, destination.x, destination.y, uvxmin, uvymin, packed_color);//1
	//addVertex(texid, destination.x, destination.y + destination.height, uvxmin, uvymax, packed_color);//2
	
	/*  0----3
	 *  |    |
	 *  |    |
	 *  1----2
	 */
	addVertex(texid, destination.x, destination.y, uvxmin, uvymin, packed_color);
	addVertex(texid, destination.x, destination.y + destination.height, uvxmin, uvymax, packed_color);
	addVertex(texid, destination.x + destination.width, destination.y + destination.height, uvxmax, uvymax, packed_color);
	addVertex(texid, destination.x + destination.width, destination.y, uvxmax, uvymin, packed_color);
}

void SpriteBatch::end ()
{
	glUseProgram(programPointer);
	
	//glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getOrthoProjection())));
	//glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
	//glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(*(Window::getInstance()->getOrthoProjection()) * glm::mat4(1.0) * glm::mat4(1.0)));

	glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

	for(auto vertices_iterator : vertices)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, vertices_iterator.first);

		//use vertex arrays instead of gl managed vbos since we are using the vertices directly of the ram
		glBindBuffer(GL_ARRAY_BUFFER, 0); //tell gl specificly that we want vertex arrays
		glVertexAttribPointer(positionAttrib, 2, GL_SHORT, GL_FALSE, sizeof(vertex_t), &(vertices_iterator.second.vertices[0].position));
		glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), &(vertices_iterator.second.vertices[0].uv));
		glVertexAttribPointer(colorAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex_t), &(vertices_iterator.second.vertices[0].color));

		//glDrawArrays(GL_TRIANGLES, 0, vertices_iterator.second.vertexCount);
		glDrawArrays(GL_QUADS, 0, vertices_iterator.second.vertexCount);

		vertices_iterator.second.vertexCount = 0;
	}

	vertices.clear();
}
