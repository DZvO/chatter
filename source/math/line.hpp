#ifndef _LINE_HPP
#define _LINE_HPP

#include "math/vector.hpp"

class Line
{
	public:
		Vector2 a, b;
		Line (Vector2 a, Vector2 b) : a(a), b(b) {}
		Line (float aa, float ab, float ba, float bb) : a(aa,ab), b(ba, bb) {}

	private:
		static inline double Dot(const Vector2& a,const Vector2& b)
		{ return (a.x*b.x) + (a.y*b.y); }
		static inline double PerpDot(const Vector2& a,const Vector2& b)
		{ return (a.y*b.x) - (a.x*b.y); }
	public:
		static bool intersects(const Line & A,
				const Line & B,
				Vector2 * out = nullptr)
		{
			Vector2 a(A.b-A.a);
			Vector2 b(B.b-B.a);

			double f = PerpDot(a,b);
			if(!f)      // lines are parallel
				return false;

			Vector2 c(B.b-A.b);
			double aa = PerpDot(a,c);
			double bb = PerpDot(b,c);

			if(f < 0)
			{
				if(aa > 0)     return false;
				if(bb > 0)     return false;
				if(aa < f)     return false;
				if(bb < f)     return false;
			}
			else
			{
				if(aa < 0)     return false;
				if(bb < 0)     return false;
				if(aa > f)     return false;
				if(bb > f)     return false;
			}

			//Vector2 intersection = ((B2 - B1) * out) + B1
			//if(out)
			// *out = 1.0 - (aa / f);
			if(out != nullptr)
			{
				float i = 1.0 - (aa / f);
				*out = Vector2(((B.b - B.a) * Vector2(i)) + B.a);
			}
			return true;
		}
};
#endif

/*

	private:
		static inline double Dot(const Vector2& a,const Vector2& b)
		{ return (a.x*b.x) + (a.y*b.y); }
		static inline double PerpDot(const Vector2& a,const Vector2& b)
		{ return (a.y*b.x) - (a.x*b.y); }
	public:
		static bool intersects(const Line & A,
				const Line & B,
				Vector2 * out = nullptr)
		{
			Vector2 a(A.b-A.a);
			Vector2 b(B.b-B.a);

			double f = PerpDot(a,b);
			if(!f)      // lines are parallel
				return false;

			Vector2 c(B.b-A.b);
			double aa = PerpDot(a,c);
			double bb = PerpDot(b,c);

			if(f < 0)
			{
				if(aa > 0)     return false;
				if(bb > 0)     return false;
				if(aa < f)     return false;
				if(bb < f)     return false;
			}
			else
			{
				if(aa < 0)     return false;
				if(bb < 0)     return false;
				if(aa > f)     return false;
				if(bb > f)     return false;
			}

			//Vector2 intersection = ((B2 - B1) * out) + B1
			//if(out)
			// *out = 1.0 - (aa / f);
			if(out != nullptr)
			{
				float i = 1.0 - (aa / f);
				*out = Vector2(((B.b - B.a) * Vector2(i)) + B.a);
			}
			return true;
		}
		*/
