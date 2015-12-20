//Ray.h By Németh Richárd 2013.05.30 11:57 ported from my C# Randominezz
#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

//namespace Randominezz
//{
	class Ray
    {
	public:
        Vec3 origin;
		Vec3 dir;

		Ray():origin(Vec3(0,0,0)),dir(Vec3(0,0,0)){}

        Ray(Vec3 origin, Vec3 dir)
        {
            //VERY IMPORTANT TO ADD SOME DELTA DIR TO POSITION, TO AVOID INVALID INTERSECTIONS ( root = 0 fighting )
            this->origin = origin;
            this->dir = dir;
        }

    };
//}
#endif