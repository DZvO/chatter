#ifndef _SPRITEBATCH_HPP
#define _SPRITEBATCH_HPP

#include <iostream>
#include <map>
#include <list>
#include "graphics/image.hpp"
#include "math/vector.hpp"
#include "math/rectangle.hpp"
#include "graphics/shaderHelper.hpp"
#include "graphics/window.hpp"
using namespace motor;

#define MAX_VERTICES 100000

class SpriteBatch
{
	public:
		SpriteBatch ();
		void begin ();
		void draw (const Image & texture, const Vector2 & position);
		void draw (const Image & texture, const Rectangle & destination, const Rectangle & uv = Rectangle(0.0, 0.0, 0.0, 0.0), const Vector4 & color = Vector4(1.0, 1.0, 1.0, 1.0), const float & rotation = 0.0, const Vector2 & origin = Vector2(0.0, 0.0), float scale = 1.0, const float & layerDepth = float(0.0));
		void end ();

	private:
		struct vertex_t
		{
			short position [2];
			float uv [2];
			unsigned int color;
		};
		struct vertex_info
		{
			vertex_t vertices [MAX_VERTICES]; //TODO use an std::vector or something else, because allocating MAX_VERTICES for maybe just one texture is a bit overkill ~~
			int vertexCount; //shows how many actual elements are in the vertex array
		};
		std::map<unsigned int /*texid*/, vertex_info /*vertexdata*/> vertices;
		inline void addVertex (unsigned int texId, const short & x, const short & y, const float & texX, const float & texY, unsigned int color);
		static unsigned int programPointer, positionAttrib, texcoordAttrib, colorAttrib, projectionUniform, viewUniform, modelUniform, texUniform;
		static unsigned int mvpUniform;
};


#endif
