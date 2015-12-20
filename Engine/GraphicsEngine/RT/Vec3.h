//Vec3.h By Németh Richárd 2013.05.30 11:58 ported from my C# Randominezz
#ifndef VEC3_H
#define VEC3_H



    class Vec3
    {
	public:
        float x;
        float y;
        float z;

        Vec3()
        {
            x = 0;
            y = 0;
            z = 0;
        }

		Vec3(float s)
		{
			this->y = s;
			this->z = s;
			this->x = s;
		}

        Vec3(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

		Vec3 operator * (float s) const
		{
			return Vec3(x * s, y * s, z * s);
		}

		Vec3 operator / (float s) const
		{
			return Vec3(x / s, y / s, z / s);
		}

        float distance(Vec3& v)
        {
			float xD = v.x - x;
			float yD = v.y - y;
			float zD = v.z - z;

            return sqrt(xD*xD+yD*yD+zD*zD);
        }

        Vec3& normalize()
        {
            float length = getLength();
			if(length != 0.0)
			{
				x /= length;
				y /= length;
				z /= length;
			}
            return *this;
        }

        static Vec3 normalize(Vec3& v)
        {
            float length = v.getLength();
			if(length != 0.0f)
				return Vec3(v.x / length, v.y / length, v.z / length);
        }

        double getLength()
        {
            return sqrt(x * x + y * y + z * z);
        }

        static Vec3 subtract(Vec3& v1, Vec3& v2)
        {
            return		Vec3(	v1.x - v2.x,
                                v1.y - v2.y,
                                v1.z - v2.z );
        }

        static Vec3 add(Vec3& v1, Vec3& v2)
        {
            return		Vec3(   v1.x + v2.x,
                                v1.y + v2.y,
                                v1.z + v2.z);
        }

        static Vec3 product(Vec3& v1, Vec3& v2)
        {
            return		Vec3(   v1.x * v2.x,
                                v1.y * v2.y,
                                v1.z * v2.z);
        }

        static Vec3 product(Vec3& v1,double s)
        {
            return		Vec3(   s * v1.x,
                                s * v1.y,
                                s * v1.z);
        }

        static Vec3 reflect(Vec3& incident, Vec3& normal)
        {
           // i - 2*n*i*n
           return Vec3::subtract(incident,Vec3::product(normal,Vec3::dot(normal,incident)*2));
        }

		Vec3& toColor()
		{
            if (x< 0)
                x = 0;
            else if (x > 255)
                x = 255;


            if (y < 0)
                y = 0;
            else if (y > 255)
                y = 255;


            if (z < 0)
                z = 0;
            else if (z > 255)
                z = 255;

            return *this;
		}

        Vec3& toNormalizedColor()
        {
            x /= 255.0;
            y /= 255.0;
            z /= 255.0;
            return *this;
        }

        static Vec3 toNormalizedColor(Vec3& v)
        {
            return Vec3(v.x / 255.0f, v.y / 255.0f, v.z / 255.0f);
        }

        Vec3& add(Vec3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vec3& subtract(Vec3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vec3& product(Vec3& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        Vec3& product(float s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        float dot(Vec3& v)
        {
            return v.x * x + v.y * y + v.z * z;
        }

        static float dot(Vec3& v1, Vec3& v2)
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }
    };
#endif