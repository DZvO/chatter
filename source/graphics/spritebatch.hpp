#ifndef _SPRITEBATCH_HPP
#define _SPRITEBATCH_HPP

#include <iostream>
#include <string>
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
		void draw (const Image & texture,
				const Rectangle & destination,
				const Rectangle & uv = Rectangle(0.0, 0.0, 0.0, 0.0),
				const Vector4 & color = Vector4(1.0, 1.0, 1.0, 1.0),
				const float & rotation = 0.0,
				const Vector2 & origin = Vector2(0.0, 0.0),
				float scale = 1.0,
				const short layer = 0);

		void drawString (const Image & font, const std::string & str, const Vector2 & pos, bool useKerning = true, float scale = 1.0, const short layer = 0);
		void drawDebug();
		void end ();

	private:
		struct vertex_t
		{
			short position [3];
			float uv [2];
			unsigned int color;
		};
		struct vertex_info
		{
			vertex_t vertices [MAX_VERTICES]; //TODO use an std::vector or something else, because allocating MAX_VERTICES for maybe just one texture is a bit overkill ~~
			int vertexCount; //shows how many actual elements are in the vertex array
		};
		inline void addVertex (unsigned int & texId, const short & x, const short & y, const float & texX, const float & texY, unsigned int & color, short depth);
		std::map<unsigned int /*texid*/, vertex_info /*vertexdata*/> vertices;

		static unsigned int programPointer, positionAttrib, texcoordAttrib, colorAttrib, projectionUniform, viewUniform, modelUniform, texUniform;
		static unsigned int mvpUniform;
		bool beginCalled;

		static std::map<unsigned int, unsigned char*> kerning;
};


#endif
