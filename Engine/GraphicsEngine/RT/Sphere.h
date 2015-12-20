//Cphere.h By Németh Richárd 2013.05.30 12:12 ported from my C# Randominezz
#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"
#include "Ray.h"
#include "IntersectionInfo.h"

//namespace Randominezz
//{
    class Sphere
    {
	public:
        Vec3 pos;

		//color
        Vec3 c;

        //radius
        double radius;
		double reflectness;

        Sphere()
		:pos(Vec3(0,0,0)),radius(0),c(Vec3(0,0,0)),reflectness(0)
        {}

        Sphere(Vec3 pos, double radius, Vec3 color, double reflectness)
        {
            this->pos = pos;
            this->radius = radius;
            this->c = color;
            this->reflectness = reflectness;
        }

		bool isIntersected(Ray& r)
        {
             //parabola megoldás.. két pont távolságából kiindulva
             double a = r.dir.x * r.dir.x + r.dir.y * r.dir.y + r.dir.z * r.dir.z;
             double b = 2 * (r.dir.x * (-pos.x + r.origin.x) + r.dir.y * (-pos.y + r.origin.y) + r.dir.z * (-pos.z + r.origin.z));
                double x_const = pos.x - r.origin.x;
                double y_const = pos.y - r.origin.y;
                double z_const = pos.z - r.origin.z;
             double c = x_const * x_const + y_const * y_const + z_const * z_const  - radius * radius;

             //check negative roots (viete formulas)
             if (-b / a < 0.0f && c / a >= -0.001)
                 return false;

            //discriminant check
            double d = b * b - 4 * a * c;
            if (d < 0.0f)
                return false;
            else
                return true;
        }

        bool isIntersected(Ray& r, IntersectionInfo& info)
        {            
            //parabola megoldás.. két pont távolságából kiindulva
            double a = r.dir.x * r.dir.x + r.dir.y * r.dir.y + r.dir.z * r.dir.z;
            double b = 2 * (r.dir.x * (-pos.x + r.origin.x) + r.dir.y * (-pos.y + r.origin.y) + r.dir.z * (-pos.z + r.origin.z));

                double x_const = pos.x - r.origin.x;
                double y_const = pos.y - r.origin.y;
                double z_const = pos.z - r.origin.z;
            double c = x_const * x_const + y_const * y_const + z_const * z_const - radius * radius;

            //check negative roots (viete formulas)
            if (-b / a < 0.0f && c / a >= -0.001)
                return false;

			//ray go through sphere, no intersection...
            if(pos.distance(r.origin) <= radius && Vec3::dot(getNormalAtPoint(r.origin),r.dir) <= 0)
                return false;

            //diszkrimináns vizsgálat
            double d = b * b - 4 * a * c;
            if (d < 0.0f)
                return false;
            else
            {
                //t1,2... -b+-sqrt(b*b -4*a*c)/2a
                //helpers..
                double denominator = 2 * a;
                double under_sqrt_upper_half = sqrt(d);

                double root1 = (-b + under_sqrt_upper_half) / denominator;
                double root2 = (-b - under_sqrt_upper_half) / denominator;

                //check correction
                if (root1 < 0.0f)
                    root1 = 9999999999.0f;
                if (root2 < 0.0f)
                    root2 = 9999999999.0f;
                

                if (root1 <= root2)
                {

                    Vec3 result(0,0,0);
                        result.add(r.dir);
                        result.product(root1);
                        result.add(r.origin);
                    info.intersectPoint = result;

                }
                else
                {

                    Vec3 result(0,0,0);
                        result.add(r.dir);
                        result.product(root2);
                        result.add(r.origin);
                    info.intersectPoint = result;
                }

                return true;
            }
        }

        Vec3 getNormalAtPoint(Vec3& p)
        {
            //non normalized sphere normal
            Vec3 normal =  Vec3(	   p.x - pos.x,
                                       p.y - pos.y,
                                       p.z - pos.z );
            //normalize
            normal.normalize();
            return normal;
        }
    };
//}
#endif